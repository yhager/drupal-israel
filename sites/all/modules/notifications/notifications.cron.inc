<?php
// $Id: notifications.cron.inc,v 1.6.2.6.2.5 2008/09/20 15:43:30 jareyero Exp $
/**
 * Subscriptions module. Queue processing.
 * 
 * Callbacks for queue processing. They may be implemented by other modules
 *    _load_user()
 *    _user_allowed()
 *    _process_send()
 * 
 * @ TO DO: Support different languages for message localization
 */

// Number of users to process for each step
define('NOTIFICATIONS_STEP_ROWS', 1000);
define('NOTIFICATIONS_STEP_USERS', 1000);

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
define('NOTIFICATIONS_TIME_MARGIN', 5);

/**
 * Function to be called on cron by the main notifications_cron
 * 
 * It will call each subscription_process for each interval a number of times
 * 
 * This should send out messages starting with immediate delivery. We send first immediate delivery
 * because the other ones can be added up for each period. 
 * Assumption: The bigger the interval, the longer delay it may admit (?) I.e. sending hourly email 
 * after 1 hour 15 mins may be ok if the system is overloaded.
 */
function notifications_process_run($cron = TRUE) {  
  notifications_log('Starting queue process');
  notifications_process('start');
  // There may be special time adjustments for cron
  if ($cron) {
    notifications_process('cron');
  }
  $stop = FALSE;
  $send_intervals = _notifications_send_intervals();
  unset($send_intervals[-1]);
  $max_sqid = notifications_process_prepare();
  foreach ($send_intervals as $interval => $name) {
    notifications_log('Processing send interval '. $name);
    while (notifications_process_queue($interval, $max_sqid)) {
      $stop = !notifications_process('check');
    }
    if ($stop) {
      notifications_log('Process stopped');
      break;
    }
  }
}

/**
 * Prepare subscriptions queue
 * 
 * This is intended to avoid race conditions where new rows are added while the process is running
 * 
 * @return
 *   Max $sqid that will be processed this cron
 */
function notifications_process_prepare($module = 'notifications') {
  // Another funny race condition may occur when the event is saved but the subs queue not yet populated  
  $expiretime = time() - 60;
  // Clean up event table.
  // @ TODO Consider a different timing for this cleanup
  db_query("DELETE FROM {notifications_event} WHERE created < %d AND eid NOT IN (SELECT eid FROM {notifications_queue})", $expiretime);
  $squid = db_result(db_query("SELECT max(sqid) FROM {notifications_queue}"));
  if ($squid) {
    // Delete non existent or blocked users
    db_query("DELETE FROM {notifications_queue} WHERE module = '%s' AND uid NOT IN (SELECT uid FROM {users} WHERE status = 1)", $module);
  }
  // @ TODO We may use this step to fill in some data or doing some other optimizations
  return $squid;
}

/**
 * Controls and checks limits for queue processing
 * It can be used by other modules to add their own limits here, like number of sms sent, etc...
 * @param $op
 *   'start' => Start the counters
 *   'cron' => Special time adjustment for cron operations
 *   'init' => Start a new counter with $value limit
 *   'option' => Sets /gets options
 *      - debug
 *      - output Enables output for admin page
 * @return
 *   TRUE if we are yet under the processing limits
 */
function notifications_process($op = 'check', $name = NULL, $value = NULL) {
  static $limit = array(), $options = array();
  static $current = array('message' => 0, 'step' => 0);

  switch ($op) {
    case 'start':
      $defaults = variable_get('notifications_process_limit', array('time' => 0, 'message' => 0, 'row' => 0, 'percent' => 0));
      foreach ($defaults as $name => $value) {
        if ($value && !isset($limit[$name])) {
          $limit[$name] = $value;
        }
      }
      break;
    case 'cron':
      // Calculate time limit. We get the smaller of all these times in seconds
      // There's an issue with poormanscron not setting the cron semaphore so it will default to current time
      $timelimit = array();
      $cronstart = variable_get('cron_semaphore', time());
      
      // Max execution time may be zero meaning no limit, then no limits based on this
      if ($maxtime = ini_get('max_execution_time')) {
        $timelimit[] =  $cronstart + $maxtime - NOTIFICATIONS_TIME_MARGIN;
        if ($limit['percent']) {
          $timelimit[] = time() + $maxtime * $limit['percent'] / 100;
          unset($limit['percent']);
        }
      }
      // This is an absolute limit, applies always if set
      if ($limit['time']) {
        $timelimit[] = time() + $limit['time'];
      }
      if ($timelimit) {
        $limit['time'] = min($timelimit);
      }
      break;

      break;
    case 'init':
      $current[$name] = 0;
      $limit[$name] = $value;
      break;
    case 'count':
      $value =  $value ? $value : 1;
      isset($current[$name]) ? ($current[$name] += $value) : $current[$name] = $value;
      break;
    case 'option':
      if (isset($value)) {
        $options[$name] = $value;
      }
      return $options[$name];
  }
  
  $current['time'] = time();
  
  // Check all limits till we find a false one
  foreach ($limit as $name => $value) {
    if ($value && !empty($current[$name]) && $current[$name] >= $value) {
      watchdog('notifications', 'Reached processing limit on queue processing: %name = %value', array('%name' => $name, '%value' => $value));
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Process rows given query conditions
 * 
 * This is used by the immediate sending feature
 * @see notifications_queue_query()
 * 
 * @param $conditions
 *   Array of query conditions
 */
function notifications_process_rows($conditions) {
  $account = NULL;
  $subscriptions = $events = $processed = array();
  $send_method = $send_interval = NULL;

  // Build query and fetch rows from queue
  $query = notifications_queue_query($conditions);
  $sql = "SELECT * FROM {notifications_queue} ";
  $sql .= " WHERE ". implode(' AND ', $query['where']);
  $sql .= " ORDER BY uid, send_method, send_interval";
  $result = db_query($sql, $query['args']);

  // Group rows by user, send_method, send_interval before sending
  // This loop has to run a final time after all rows have been fetched
  while (($queue = db_fetch_object($result)) || $processed) {
    if (!$account || !$queue || ($queue->uid != $account->uid) || $queue->send_method != $send_method || $queue->send_interval != $send_interval) {
      // New user or sending method, send if not the first row and reset
      if ($account && $events && $subscriptions) {
        notifications_process_send($account, $events, $subscriptions, $send_method, $send_interval);
        notifications_update_sent($account->uid, $send_method, $send_interval, time());
      }
      if ($processed) {
        notifications_queue_delete(array('sqids' => $processed));        
      }
      $subscriptions = $events = $processed = array();
      if ($queue) {
        $account = notifications_load_user($queue->uid);
        $send_method = $queue->send_method;
        $send_interval = $queue->send_interval;
      }
    }
    if ($queue) {
      $event = notifications_load_event($queue->eid);
      if (notifications_user_allowed('event', $account, $event)) {
        $events[$queue->eid] = $event;
        $subscriptions[$queue->eid][] = $queue->sid;
      }
      $processed[] = $queue->sqid;
    }
  }
}

/**
 * Process subscriptions queue
 * 
 * The subscriptions queue has the following fields
 * sqid, uid, eid, sid, digest
 * 
 * This function should be able of splitting the whole processing in several steps.
 * It will be called multiple times for each send interval
 * 
 * Messages will be processed for each send interval, send_method, user
 * 
 * @param $send_interval
 *   Send interval to process
 * @param $max_sqid
 *   Max queue id to process
 * @param $module
 *   Module's rows to process. There may be other types of rows sitting on the table
 * @return Number of rows processed
 * 
 * @ TODO Review time conditions
 */
function notifications_process_queue($send_interval, $max_sqid, $module = 'notifications') {
  $count = 0;
  // This is the time from which stored rows will be sent
  $timelimit = time() - $send_interval;

  // Get users to process messages for, with this time interval and ordered by squid
  // Order by last sent for this send interval
  // Note: If we get the users with more messages pending first this may save some time
  $sql = "SELECT q.uid, q.send_method, count(*) AS count FROM {notifications_queue} q ";
  $sql .= " LEFT JOIN {notifications_sent} su ON q.uid = su.uid AND q.send_interval = su.send_interval AND q.send_method = su.send_method ";
  $sql .= " WHERE q.cron = 1 AND q.send_interval = '%d' AND q.sqid <= %d AND q.module = '%s'";
  $sql .= " AND (su.uid IS NULL OR su.sent < %d) ";
   // Note: the group by su.sent seems to be needed by pgsql
  $sql .= " GROUP BY q.uid, q.send_method, su.sent ORDER BY su.sent";
  $result = db_query_range($sql, $send_interval, $max_sqid, $module, $timelimit, 0, NOTIFICATIONS_STEP_USERS);

  $sqid = 0;
  // @ TODO Add time conditions
  while (($user = db_fetch_object($result)) && notifications_process('check')) {
    notifications_log("Processing user $user->uid, rows $user->count, send_method $user->send_method");
    $events = $subscriptions = $processed = array();    
    $send_method = $user->send_method;
    // Users may be handled by a different module
    $account = notifications_callback($module, 'load_user', $user->uid);

    // Process all rows for this user. With some hard limit to prevent process lock ups.
    $result_subs = db_query_range("SELECT * FROM {notifications_queue} WHERE cron = 1 AND send_interval = '%d' AND uid = %d AND sqid <= %d ORDER BY send_method, sqid", $send_interval, $account->uid, $max_sqid, 0, NOTIFICATIONS_STEP_ROWS);
    while (($queue = db_fetch_object($result_subs)) && notifications_process('count', 'row')) {
      $count++;
      $processed[] = $sqid = $queue->sqid;      
      // Load event and check access
      $event = notifications_load_event($queue->eid, TRUE);
      if (notifications_callback($module, 'user_allowed', 'event', $account, $event)) {
        // This will take care of duplicated events
        $events[$queue->eid] = $event;
        // We keep track also of subscriptions originating this event
        $subscriptions[$queue->eid][] = $queue->sid;
        notifications_log("Processing queued sid=$queue->sid event=$queue->eid ($event->type, $event->action) send_method=$send_method");
      } else {
        notifications_log("Access denied for queued event sid=$queue->sid event=$queue->eid ($event->type, $event->action)");
      }
    }
    if ($events) {
      notifications_callback($module, 'process_send', $account, $events, $subscriptions, $send_method, $send_interval);
      notifications_callback($module, 'update_sent', $user->uid, $send_method, $send_interval, time());  
    }
    if ($processed && !notifications_process('option', 'debug')) {
       notifications_queue_delete(array('uid' => $user->uid, 'send_interval' => $send_interval, 'send_method' => $send_method, 'max_sqid' => $sqid));      
    }
  }
  
  return $count;
}

/**
 * Update user last time sent
 */
function notifications_update_sent($uid, $method, $interval, $time) {
  db_query("UPDATE {notifications_sent} SET sent = %d WHERE uid = %d AND send_interval = '%d' AND send_method = '%s'", $time, $uid, $interval, $method);
  if (!db_affected_rows()) {
    db_query("INSERT INTO {notifications_sent}(uid, send_interval, send_method, sent) VALUES(%d, '%d', '%s', %d)", $uid, $interval, $method, $time);
  }
}
/**
 * Message delivery.
 * 
 * Processes everything, included digestion and sends message/s.
 * 
 * Adds some more information into $message['notifications'] that may be used by other modules
 *
 * @param $account
 *   User account to send the notification to
 * @param $events
 *   Array of loaded event objects to be processed
 * @param $subscriptions
 *   Array of arrays of subscription ids (sids) for each event(eid)
 */
function notifications_process_send($account, $events, $subscriptions, $send_method, $send_interval) {
  notifications_log("Sending out, method=$send_method, interval=$send_interval, events=". count($events));
  // Digest if send_interval > 0 (not immediate sending)
  if ($send_interval) {
     $message = notifications_process_digest($account, $events, $send_interval, $send_method);
     $message['notifications'] = array('events' => $events, 'subscriptions' => $subscriptions, 'digest' => TRUE);
     $messages[] = $message;
  }
  else {
    $sender_option = variable_get('notifications_sender', 0);
    foreach ($events as $event) {
      $message = notifications_process_message($account, $event, $subscriptions[$event->eid], $send_method);
      // We pass on the full information so it can be used by modules implementing some of the hooks
      $message['notifications'] = array('events' => array($event), 'subscriptions' => $subscriptions);
      // Optional sender, if chosen will be the user account who produced the event
      // It will be up to the sending method modules what to do with this information.
      if ($sender_option) {
        $sender = notifications_load_user($event->uid);
        $message['sender_name'] = $sender->name;
        if ($sender_option == 2) {
          $message['sender_account'] = $sender;
        }
      }
      $messages[] = $message;
    }
  }
  // Now send messages
  foreach ($messages as $message) {
    notifications_process('count', 'send');
    // notifications_log($message, 'message');
    notifications_message_send($account, $message, $send_method);
  }
}

/**
 * Digest multiple events in a single message
 * 
 * @return array with message 'subject' and 'body' elements
 */
function notifications_process_digest($account, $events, $send_interval, $send_method) {
  // Compile list of events for each object
  $list = array();

  // Build up the digested list with text replacement
  // We need text replacement for each line because it depends on different objects
  foreach ($events as $event) {
    notifications_log($event, 'digesting');
    $info = notifications_event_types($event->type, $event->action);
    $digest_type = $info['digest'][0];
    $digest_field = $info['digest'][1];
    $digest_object = $event->objects[$digest_type];
    $digest_value = $digest_object->$digest_field;
    if (!isset($list[$digest_type][$digest_value]['name'])) {
      $name = $list[$digest_type][$digest_value]['name'] = notifications_text_replace($info['name'], array($digest_type => $digest_object));
      notifications_log("Digesting object $name (type=$digest_type value=$digest_value)");
    }
    // Check duplicate notifications for the same event
    if (!isset($list[$digest_type][$digest_value]['line'][$event->eid])) {
      $line = !empty($event->text['digest']) ? $event->text['digest'] : $info['line'];
      $list[$digest_type][$digest_value]['line'][$event->eid] = notifications_text_replace($line, $event->objects);
    }
  }
  // Create message. Do all this in one replacement, then strip out the subject
  $text['subject'] = notifications_message_part('digest', 'subject', $send_method);
  $text['header'] = notifications_message_part('digest', 'header', $send_method);
  $text['footer'] = notifications_message_part('digest', 'footer', $send_method);
  // We dont pass a subscription object here, won't be too much use anyway
  $text = notifications_text_replace($text, array('user' => $account, 'subscription' => NULL));
  
  // Compose body. All these lines have been text replaced
  $body = theme('notifications_digest_body', $text, $list);
  
  return array('subject' => $text['subject'], 'body' => $body);
}

/**
 * Creates a single message for a single event
 * 
 * @param $account
 *   Destination user account
 * @param $event
 *   Event object which caused this notification
 * @param $subscriptions
 *   Array of subscription ids
 * 
 * @return
 *   Message array 
 */
function notifications_process_message($account, $event, $subscriptions, $send_method) {
  $info = notifications_event_text($event);

  // Create message. Do all this in one replacemente
  $text = array(
    'subject' => notifications_message_part('event', 'subject', $send_method, $event),
    'header' => notifications_message_part('event' , 'header', $send_method, $event),
    'event'  => notifications_message_part('event', 'main', $send_method, $event),
    'footer' => notifications_message_part('event', 'footer', $send_method, $event),
  );

  // We pass only the first subscription, which is at least something
  // @ TODO Handle nicely the case where there are more than one subscription
  if ($sid = array_shift($subscriptions)) {
    $subscription = notifications_load_subscription($sid);
  }
  else {
    $subscription = NULL;
  }
  $objects = array('user' => $account, 'event' => $event, 'subscription' => $subscription);
  $objects = array_merge($objects, $event->objects);
  $text = notifications_text_replace($text, $objects);

  // Get subject out of text and build the message array
  $subject = $text['subject'];
  unset($text['subject']);
  return array('subject' => $subject, 'body' => $text);
}

/**** Retrieving and replacing text parts, interfacing with tokens and messaging module ****/

/**
 * Get message part
 * 
 * It searches for optional message group keys for options defaulting to $type
 * 1. $type-[$event->type]-[$event->action]
 * 2. $type-[$event->type]
 * 3. $type
 * 
 * @param $type
 *   Message type to send, either 'event' or 'digest'
 * @param $key
 *   Id of message part, ie 'header'
 * @param $method
 *   Method by which message will be sent. Normally 'mail'
 * @param $event
 *   Event data
 *
 * @return
 *   Part of the message.
 *
 */
function notifications_message_part($type, $key, $method, $event = NULL) {
  // If event passed check for predefined text or get optional keys from event
  if ($event && isset($event->text[$key])) {
    return $event->text[$key];  
  }
  $options = $event ? array($event->type, $event->action) : array();
  $keyparts = array_merge(array('notifications-'. $type), $options);
  // Output some debugging info in case we dont find a suitable message part
  $output = "[UNDEFINED type = $type, method = $method, key = ". implode('-', $keyparts) .']';
  while ($keyparts) {
    $groupkey = implode('-', $keyparts);
     if ($text = messaging_message_part($groupkey, $key, $method)) {
      $output = $text == MESSAGING_EMPTY ? '' : $text;
      break;
    }
    // If no text trim out latest part of the key and retry
    array_pop($keyparts);
  }
  return $output;
}

/**
 * Text replacement with tokens
 */
function notifications_text_replace($text, $objects) {
  // Add some token types
  $objects['global'] = NULL;  
  return token_replace_multiple($text, $objects);
}

/**
 * Message sending
 */
function notifications_message_send($account, $message, $send_method) {
  $message['type'] = 'notifications';
  notifications_process('count', 'message');
  messaging_message_send_user($account, $message, $send_method);
}

/**
 * Get texts for event
 * 
 * @ TODO Support for configurable texts
 */
function notifications_event_text($event) {
  $info = notifications_event_types($event->type, $event->action);
  return $info;
}

/**
 * Get users with static caching
 */
function notifications_load_user($uid) {
  return messaging_load_user($uid);
}

/**
 * Get events with static caching
 */
function notifications_load_event($id) {
  static $cache = array();
  if (!array_key_exists($id, $cache)) {
    $event = db_fetch_object(db_query("SELECT * FROM {notifications_event} WHERE eid = %d", $id));
    $event->params = unserialize($event->params);
    // Load aditional objects for the event
    $event->objects = array();
    notifications_module_invoke('event load', $event);
    $cache[$id] = $event;
  }
  return $cache[$id];
}

/**
 * Delete rows from subscriptions queue
 * 
 * @see notifications_queue_query()
 * 
 * Note: Handle with care if no params may delete all rows
 */
function notifications_queue_delete($params) {
  $query = notifications_queue_query($params);
  db_query("DELETE FROM {notifications_queue} WHERE ". implode(' AND ', $query['where']), $query['args']);
}

/**
 * Build query conditions for queue queries
 * 
 * @param $params
 *   Array of parameters, field => value form
 *   Special parameters
 *     'max_squid' => max squid to delete
 *     'rows' => array of squid values to delte
 * @return
 *   Array with 'where' and 'args' elements. Each of them is an array
 */
function notifications_queue_query($params) {
  $where = $args = array();
  
  foreach ($params as $field => $value) {
    switch ($field) {
      case 'max_sqid':
        $where[] = "sqid <= %d";
        $args[] = $value;
        break;
      case 'sqids':
        $where[] = "sqid IN (%s)";
        $args[] = implode(',', array_map('db_escape_string', $value));
        break;      
      default:
        $where[] = "$field = '%s'";
        $args[] = $value;
        break;
    }
  }  
  return array('where' => $where, 'args' => $args);
}

/**
 * Log for debugging
 */
function notifications_log($info = NULL, $type = 'info') {
  static $logs;
  if ($info) {
    $message = $type .': ';
    $message .= is_string($info) ? $info : print_r($info, TRUE);
    $logs[] = $message;
    if ($type == 'watchdog') {
      // Do not localize watchdog entry
      watchdog('notifications', $info, NULL);
    }
  }
  else {
    return $logs;
  }
}

/** Themeable functions **/

/**
 * Theme notifications digest
 * 
 * @param $text
 *   Array with message parts, currently only 'header' and 'footer'
 * @param $list
 *   Structured array with list of digested items. For each object type
 *   'type' => (  // Type may be node, user, etc...
 *      'oid' => ( // One for each object, may be nid, uid...
 *        'name' => Sub header for this digest section, object name
 *        'line' => Array of lines, one for each related event
 *       )
 *   )   
 * @return
 *   Structured array with 'header', 'footer', and multiple text lines
 */
function theme_notifications_digest_body($text, $list) {
  $body['header'] = $text['header'];
  foreach ($list as $type => $objects) {
    foreach ($objects as $oid => $data) {
      $body[] = $data['name'];
      foreach ($data['line'] as $line) {
        $body[] = '- '. $line;
      }
    }
  }
  $body['footer'] = $text['footer'];
  return $body;
}