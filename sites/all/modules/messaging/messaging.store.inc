<?php
// $Id: messaging.store.inc,v 1.1.2.4.2.1 2008/04/12 18:40:14 jareyero Exp $
/**
 * @file
 *   Database storage for the messaging framework
 */

// Max number of rows to process for each step before clean up
define('MESSAGING_STEP_ROWS', 1000);

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
define('MESSAGING_TIME_MARGIN', 5);

/**
 * Process and send messages in queue, to be called from cron
 * 
 * It will check for predefined limits and repeat the cycle
 *   [fetch] -> [send] -> [check]
 * until the queue is empty or any of the limits are met
 */
function messaging_store_queue_process() {
  $limit = variable_get('messaging_process_limit', array('message' => 0, 'time' => 0, 'percent' => 0));
  // Calculate time limit. We get the smaller of all these times in seconds
  $timelimit[] = variable_get('cron_semaphore', 0) + ini_get('max_execution_time') - MESSAGING_TIME_MARGIN;
  if ($limit['time']) {
    $timelimit[] = time() + $limit['time'];
  }
  if ($limit['percent']) {
    $timelimit[] = time() + ini_get('max_execution_time') * $limit['percent'] / 100;
    unset($limit['percent']);
  }
  $limit['time'] = min($timelimit);
  
  // Processing loop. Will stop when we run out of rows or reach time limit
  do {
    $number = messaging_store_queue_process_step(MESSAGING_STEP_ROWS, $limit['time']);    
  } 
  while ($number == MESSAGING_STEP_ROWS && time() <= $limit['time']);    
}

/**
 * Retrieve and send queued messages
 * 
 * @param $limit
 *   Maximum number of queued messages to process for this step
 * @param $timeout
 *   Optional time limit for processing, will return when if reached during processing
 * @return
 *   Number of messages processed in this step
 */
function messaging_store_queue_process_step($limit, $timeout = 0) {
  $count = 0;
  $sent = $unsent = array();
  $result = db_query_range("SELECT * FROM {messaging_store} WHERE queue = 1 AND cron = 1 ORDER BY mqid", 0, $limit);
  while ($message = db_fetch_array($result)) {
    messaging_store_unpack($message, TRUE);
    // Actual send function
    if (messaging_message_send_out($message['destination'], $message, $message['method'])) {
      $sent[] = $message['mqid'];
    } else {
      $unsent[] = $message['mqid'];
    }
    $count++;
    if ($timeout && time() > $timeout) break;
  }
  if ($sent) {
    messaging_store_sent($sent);
  }
  if ($unsent) {
    messaging_store_sent($unsent, TRUE);
  }
  return $count;
}

/**
 * Queue clean up
 * - Remove expired logs
 * - @ TODO Remove expired queued messages
 */
function messaging_store_queue_cleanup() {
  if ($expire = variable_get('messaging_log_expire', 0)) {
    db_query('DELETE FROM {messaging_store} WHERE log = 1 AND queue = 0 AND sent < %d', time() - $expire);
  }
}

/**
 * Get pending messages for method
 * 
 * @see messaging_pull_pending()
 */
function messaging_store_pull_pending($method, $users, $limit = 0, $delete = TRUE) {
  $messages = messaging_store_get(array('method' => $method, 'uid' => $users));
  // Not exactly delete but mark as sent
  if ($messages && $delete) {
    messaging_store_sent(array_keys($messages));
  }
  return $messages;
}

/**
 * Retrieve from messaging database storage
 * 
 * @param $params
 *   Array of field value pairs
 * @param $order
 *   Optional array of field names to order by
 * @param $limit
 *   Optional maximum number of rows to retrieve
 * @param $pager
 *   Optional pager element for pager queries
 * @param $unpack
 *   Optional fully load stored data
 */
function messaging_store_get($params, $order = NULL, $limit = NULL, $pager = NULL, $unpack = FALSE) {
  $messages = $where = $args = array();
  list ($where, $args) = messaging_store_query($params);
  $sql = 'SELECT * FROM {messaging_store}';
  $sql .= $where ? ' WHERE '.implode(' AND ', $where) : '';
  $sql .= $order ? ' ORDER BY '.implode(', ', $order) : '';

  if (!is_null($pager)) {
    $result = pager_query($sql, $limit, $pager, NULL, $args);
  } elseif ($limit) {
    $result = db_query_range($sql, $args, 0, $limit);
  } else {
    $result = db_query($sql, $args);
  }
  while ($msg = db_fetch_array($result)) {
    messaging_store_unpack($msg, $unpack);
    $messages[$msg['mqid']] = $msg;
  }
  return $messages;
}

/**
 * Build query with field conditions
 * 
 * This function supports IN() conditions when passing array field values
 * @param $query
 *   Array of field => value pars
 */
function messaging_store_query($fields) {
  $where = $args = array();
  foreach ($fields as $key => $value) {     
    if (is_array($value)) {
      // Special processing for array parameters. Many ints are expected for 'mqid' field
      $placeholder = ($key == 'mqid') ? '%d' : "'%s'";
      $where[] = "$key IN (". implode(',', array_fill(0, count($value), $placeholder)). ')';
      $args = array_merge($args, $value);
    } else {
      $where[] = $key." = '%s'";
      $args[] = $value;
    }
  }
  return array($where, $args);
}

/**
 * Unpack stored messages
 * 
 * @param $message
 *   Array as retrieved from the db store
 * @param $full
 *   True for loading the account data if this message is intended for a user
 */
function messaging_store_unpack(&$message, $full = FALSE) {
  // Preprocessing stored parameters
  if ($message['params']) {
    $params = unserialize($message['params']);
    // Some optional fields that may be into params, may be extended
    foreach (array('destination', 'sender_name') as $field) {
      if (!empty($params[$field])) {
        $message[$field] = $params[$field];
        unset($params[$field]);
      }
    }
    // We only saved params for current sending method group
    $group = messaging_method_info($message['method'], 'group');
    $message['params'][$group] = $params;
  }
  if ($message['uid'] && $full) {
    $message['account'] = messaging_load_user($message['uid']);       
  }
  if ($message['sender'] && $full) {
    $message['sender_account'] = messaging_load_user($message['sender']);
  }
}

/**
 * Mark messages as sent, either deleting them, or keeping logs
 * 
 * @param $mqid
 *   Single message id or array of message ids
 * @param $log
 *   Optional, just move queue messages to log, for messages on which sending failed 
 */
function messaging_store_sent($mqid, $log = FALSE) {
  $mqid = is_array($mqid) ? $mqid : array($mqid);
  list($where, $args) = messaging_store_query(array('mqid' => $mqid));
  // First delete the ones that are not for logging
  if (!$log) {
    db_query("DELETE FROM {messaging_store} WHERE log = 0 AND ".implode(' AND ', $where) , $mqid);
  }
  // Now mark the rest as sent
  $args = array_merge(array(time()), $mqid);
  db_query("UPDATE {messaging_store} SET queue = 0, log = 1, sent = %d WHERE ".implode(' AND ', $where), $args);
}

/**
 * Delete messages from queue
 */
function messaging_store_del($params) {
  list($where, $args) = messaging_store_query($params);
  db_query("DELETE FROM {messaging_store} WHERE ".implode(' AND ', $where), $args);
}

/**
 * Put into database storage, create one line for each destination
 * 
 * @ TODO See about guessing users from $destination object
 * 
 * @param $method
 *   Sending method
 * @param $destinations
 *   Array of destinations, the type of elements will depend on sending method
 * @param $message
 *   Message array
 * @param $sent
 *   Sent timestamp when used for logging
 * @param $queue
 *   Should be 1 when this is a regular queue entry
 * @param $log
 *   Should be 1 when this entry is to be kept as a log
 * @param $cron
 *   Should be 1 when this entry is to be processed on cron (queueing for push methods)
 */
function messaging_store_save($method, $destinations, $message, $sent = 0, $queue = 0, $log = 0, $cron = 1) {
  // Add some defaults so fields are populated
  $message += array('account' => NULL, 'sender' => 0);
  // If sender is a user account, save sender field
  if (!empty($message['sender_account'])) {
    $message['sender'] = $message['sender_account']->uid;
  }
  // We just save the params for current sending method group
  $group = messaging_method_info($method, 'group');
  $params = !empty($params[$group]) ? $params[$group] : array();
  // And there's one more optional param that is sender_name
  if (!empty($message['sender_name'])) {
    $params['sender_name'] = $message['sender_name'];
  }
  foreach ($destinations as $destination) {
    // Mark for a user if there's an account parameter, produced by messaging_send_user()
    if ($message['account']) {
      $uid =  $message['account']->uid;
    } elseif (is_object($destination) && isset($destination->uid)) {
      $uid = $destination->uid;
    } else {
      $uid = 0;
    }
    // Destination may be an object or an array, serialize if so
    if (is_object($destination) || is_array($destination->uid)) {
      $params['destination'] = $destination;
      $destination = '';
    } 
    db_query("INSERT INTO {messaging_store} (method, uid, sender, destination, created, sent, queue, log, cron, subject, body, params)".
    " VALUES('%s', %d, %d, '%s', %d, %d, %d, %d, %d, '%s', '%s', '%s')",
      $method, $uid, $message['sender'], $destination, time(), $sent, $queue, $log, $cron, $message['subject'], $message['body'], $params ? serialize($params) : '');
  }
}