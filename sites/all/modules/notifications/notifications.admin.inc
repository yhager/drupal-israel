<?php
// $Id: notifications.admin.inc,v 1.5.2.6.2.8 2008/09/20 15:43:30 jareyero Exp $

/**
 * Admin settings
 */
function notifications_settings_form() {
  $form['sub_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#weight' => -10,
  );
  $form['sub_settings']['notifications_sendself'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Notify poster of own posts'),
    '#default_value' => variable_get('notifications_sendself', 0),
    '#description'   => t("Notifies a node poster about their own posts.  Useful principally during testing.  Default is OFF."),
  );
  $form['sub_settings']['notifications_send_immediate'] = array(
    '#title' => t('Immediate sending'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('notifications_send_immediate', 0),
    '#description' => t('Do not queue notifications for immediate sending. This will produce more timely notifications for sites with a small number of users.'),
  );
  $form['sub_settings']['notifications_sender'] = array(
    '#title' => t('Notifications Sender'),
    '#type' => 'radios',
    '#options' => array(
      t('No one (All notifications will appear as coming from the web site)'),
      t('User name, site data (Only the user name will be used)'),
      t('Full user data (User name and available user information)'),
    ),
    '#default_value' => variable_get('notifications_sender', 0),
    '#description' => t('Use the site information as the sender for notification messages or use account data from the user causing the event. WARNING: Selecting the last option (Full user data) may undisclose private information to subscribers like the user e-mail address.'),
  );
  // Default options
  $form['defaults'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default settings'),
  );
  $form['defaults']['notifications_default_send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Default send interval'),
    '#options' => _notifications_send_intervals(),
    '#default_value' => variable_get('notifications_default_send_interval', 0),
  );

  // Processing limits
  $limit = variable_get('notifications_process_limit', array('row' => 0, 'message' => 0, 'percent' => 0, 'time' => 0));
  $form['notifications_process_limit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Limits for queue processing'),
    '#tree' => TRUE,
    '#description' => t('These are the limits for each cron run on queue processing. The process will stop when it first meets any of them. Set to 0 for no limit.'),
  );
  $form['notifications_process_limit']['row'] = array(
    '#title' => t('Number of rows'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['row'],
  );
  $form['notifications_process_limit']['message'] = array(
    '#title' => t('Number of messages sent'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['message'],
  );
  $form['notifications_process_limit']['time'] = array(
    '#title' => t('Time (seconds)'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['time'],
  );
  $form['notifications_process_limit']['percent'] = array(
    '#title' => t('Time (% of cron time)'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['percent'],
    '#description' => t('Maximum percentage of cron time the process may use.'),
  );
  return system_settings_form($form);
}

/**
 * Send intervals administration
 */
function notifications_send_intervals_form() {
  // Build options
  $units = array(
    60 => t('Minutes'),
    60*60 => t('Hours'), 
    24*60*60 => t('Days'),
  );
  $form['intervals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send intervals'),
    '#tree' => TRUE,
    '#theme' => 'notifications_send_intervals',
    '#description' => t('To delete an interval, set the time unit and the name empty.'),
  );
  $intervals = _notifications_send_intervals() + array('new1' => '', 'new2' => '');
  $index = 0;
  foreach ($intervals as $time => $name) {
    // Calculate value and unit
    $current = $number = 0; 
    if (!is_numeric($time)) {
      $number = $unit = '';
    }
    elseif ($time >= 0) {
      foreach (array_reverse(array_keys($units)) as $unit) {
        if ($time % $unit == 0) {
          $current = $unit;
          $number = $time / $unit;
          break;
        }
      }
    }
    else {
      // May be -1 for 'Never'
      $number = $time;
      $unit = '';
    }
    $form['intervals'][$index]['time'] = array(
      '#default_value' => $number,
      '#type' => 'textfield',
      '#size' => 2,
    );
    $form['intervals'][$index]['unit'] = array(
      '#type' => 'select',
      '#options' => array('' => '') + $units,
      '#default_value' => $unit,
    );
    $form['intervals'][$index]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => $name,
    );
    $index++;
  }
  // New row
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));
  $form['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));

  return $form;
}

/**
 * Form submit for time intervals
 */
function notifications_send_intervals_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  if ($form_values['op'] == t('Reset to defaults')) {
    variable_del('notifications_send_intervals');
  }
  else {
    $intervals = array();
    foreach ($form_values['intervals'] as $index => $values) {
      if (is_numeric($values['time']) && $values['name']) {
        $unit = $values['unit'] ? (int)$values['unit'] : 1;
        $intervals[(int)$values['time'] * $unit] = $values['name'];
      }
    }
    ksort($intervals);
    variable_set('notifications_send_intervals', $intervals);
  }
  drupal_set_message(t('The time intervals for your subscriptions have been updated'));
}

/**
 * Build a table with send intervals
 */
function theme_notifications_send_intervals($element) {
  $output = '';
  $header = array(array('data' => t('Time'), 'colspan' => 2), t('Display name'));
  foreach (element_children($element) as $key) {
    $rows[] = array(
      drupal_render($element[$key]['time']),
      drupal_render($element[$key]['unit']),
      drupal_render($element[$key]['name']),
    );  
  }
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($element);
  return $output;
}

/**
 * Event configuration administration
 * 
 * Will allow to define which events trigger a notification and which ones not
 */
function notifications_admin_events_form() {
  // Compile array from plug-ins and settings
  $events = notifications_event_types();
  $current = variable_get('notifications_events', array());

  if ($events) {
    $form['notifications_events'] = array(
      '#title' => t('Enabled events'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#description' => t('Check the events for which notifications should be triggered.'),
    );
    foreach ($events as $object => $object_info) {
      foreach ($object_info as $action => $action_info) {
        $form['notifications_events'][$object][$action] = array(
          '#type' => 'checkbox',
          '#title' => !empty($action_info['description']) ? $action_info['description'] : "$object:$action",
          '#default_value' => isset($current[$object][$action]) ? $current[$object][$action] : 1,
        );
      }
    }
    return system_settings_form($form);
  } else {
    $form['warning'] = array('#value' => t('You need to enable some plug-ins to provide notifications events.'));
    return $form;
  }  
}

/* ********************************************t*********** */
/*  user screens: display, edit functions */
/* ******************************************************* */

/**
 * Theme subscriptions list
 */
function theme_notifications_form_table($element) {
  $output = '';
  if ($fields = element_children($element)) {
    $header = $element['#header'];
    $rows = array();
    // The first element determines the number of columns
    foreach (element_children($element[$fields[key($fields)]]) as $index) {
      $row = array();
      foreach ($fields as $key) {
        $row[] = isset($element[$key][$index]) ? drupal_render($element[$key][$index]) : '';
      }
      $rows[] = $row;
    }
    $output .= theme('table', $header, $rows);
  }
  $output .= drupal_render($element);
  return $output;
}

/**
 * Menu callback. Overview page for user subscriptions.
 */
function notifications_page_user_overview($account) {
  // Build summary
  $count = array();  
  // Query all subscriptions for this user
  $result = db_query("SELECT s.type, count(s.sid) as number FROM {notifications} s WHERE s.uid = %d  GROUP BY s.type", $account->uid);
  while ($subs = db_fetch_object($result)) {
    $count[$subs->type] =  $subs->number;
  }
  $header = array(t('Type'), t('Number'));
  $rows = array();
  
  // List types and count for each type
  foreach (notifications_subscription_types() as $type => $info) {
    $access = user_access('administer notifications') || !empty($info['access']) && user_access($info['access']);
    // If no access and no count, skip this type.
    // But if no access and there are some we show the type and number without link.
    if ($access || !empty($count[$type])) {
      $rows[] = array(
         $access ? l($info['title'], "user/$account->uid/notifications/$type") : $info['title'],
         isset($count[$type]) ? $count[$type] : 0,
      );
    }
  }

  return $rows ? theme('table', $header, $rows) : t('No existing or allowed subscriptions');
}

/** Administration pages **/

/**
 * Current subscriptions page
 */
function notifications_admin_status_page() {
  $output = '';
  // Subscriptions summary
  $header = array(t('Type'), t('Number'));
  $result = db_query("SELECT type, count(*) AS count FROM {notifications} GROUP BY type");
  $count = 0;
  while ($stype = db_fetch_object($result)) {
    $rows[] = array($stype->type, $stype->count);
    $count += $stype->count;
  }
  $summary = theme('table', $header, $rows);
  $summary .= t('Total: %number', array('%number' => $count));
  $output .= theme('box', t('Current subscriptions'), $summary);
  $output .= notifications_admin_queue_summary();
  return $output;  
}

/**
 * Summary of queued notifications
 */
function notifications_admin_queue_summary() {
  $output = '';
  // Queue status
  $send_intervals = _notifications_send_intervals();
  $header = array(t('Send interval'), t('Number'));
  $count = 0;
  $result = db_query("SELECT send_interval, count(*) AS count FROM {notifications_queue} GROUP BY send_interval");    
  while ($stype = db_fetch_object($result)) {
    $rows[] = array($send_intervals[$stype->send_interval], $stype->count);
    $count += $stype->count;
  }
  $output .= theme('table', $header, $rows);
  $output .= t('Total: %number', array('%number' => $count));
  return theme('box', t('Messages in queue'), $output);
}

/**
 * Admin queue management
 * 
 * @ TO DO Add confirmation before queue reset
 */
function notifications_admin_queue($op = 'status', $param = NULL) {
  $base = 'admin/messaging/notifications-status/queue';
  $output = '';
  switch ($op) {
    case 'run':
      $out = notifications_admin_queue_process($param);
      if ($out) {
        $output .= theme('box', t('Output'), $out);
      }
      break;
    case 'reset':
      db_query("DELETE FROM {notifications_queue}");
      db_query("DELETE FROM {notifications_event}");
      drupal_set_message(t('The queue has been reset.'));
      drupal_goto($base);
      break;
    default:
  }
  // Add operations
  $list[] = l(t('Run queue process'), "$base/run");
  $list[] = l(t('Process immediate sending'), "$base/run/immediate");
  $list[] = l(t('Reset queue. Delete all notifications.'), "$base/reset");

  $output .= theme('box', t('Operations'), theme('item_list', $list));
  // Summary
  $output .= notifications_admin_queue_summary();
  return $output;
}

/**
 * Admin manual queue processing
 */
function notifications_admin_queue_process($param) {
  include_once drupal_get_path('module', 'notifications') .'/notifications.cron.inc';
  // Set some running parameters
  switch  ($param) {
    case 'immediate':
      notifications_process_rows(array('cron' => 1, 'send_interval' => 0));
      break;
    case 'debug':
      notifications_process('option', 'debug', TRUE);
      notifications_process('option', 'output', TRUE);
      notifications_process_run(FALSE);
      break;
    default:
      notifications_process_run(FALSE);
  }
  // Go for it, not cron run

  if ($logs = notifications_log()) {
    return theme('item_list', $logs);
  }
}

/**
 * Menu callback add subscription
 * 
 * Presents confirmation page or not depending on confirm parameter
 */
function notifications_page_subscribe($account, $type, $fields, $values, $send_interval = NULL, $send_method = NULL) {
  global $user;

  // Check signature if present
  $params = array('subscribe', $account->uid, $type, $fields, $values);
  $signed = !empty($_GET['signature']) && $_GET['signature'] == _notifications_signature($params, !empty($_GET['confirm']));
  if (!empty($_GET['signature'])) drupal_set_message(_notifications_signature($params, !empty($_GET['confirm'])));
  // Build subscriptions object
  $subscription = (object)array(
    'uid' => $account->uid,
    'type' => $type,
    'fields' => notifications_field_args($fields, $values),
    'send_interval' => $send_interval ? $send_interval : notifications_user_setting('send_interval', $account),
    'send_method' => $send_method ? $send_method : notifications_user_setting('send_method', $account),
    'event_type' => notifications_subscription_types($type, 'event_type'),
  );      
  if (notifications_user_allowed('subscription', $account, $subscription)) {
    // Display subscription information
    if (!empty($_GET['confirm']) && $signed) {
      // Subscribe, no confirmation
      notifications_save_subscription($subscription);
      drupal_set_message(t('Your subscription was activated.'));
      drupal_goto();
    }
    else {
      // Ask for confirmation
      drupal_set_title(t('Confirm your subscription'));
      return drupal_get_form('notifications_form_subscribe_confirm', $subscription, $account);
    }
  }
  else {
    drupal_set_message(t('Subscription type or parameters not allowed'), 'error');
    drupal_goto();
  }
    
  drupal_access_denied();
}

/**
 * Form for subscription confirmation
 */
function notifications_form_subscribe_confirm($form_state, $subscription, $account) {

  // Pass on simple values
  foreach (array('sid', 'uid', 'type', 'fields', 'event_type') as $field) {
    $form[$field] = array('#type' => 'value', '#value' => $subscription->$field);
  }

  // The names will be added here
  notifications_module_invoke('names', $subscription);

  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('!type subscription to', array('!type' => $subscription->type_name)),
    '#value' => theme('item_list', $subscription->names),
  );

  // Additional parameters
  $form['send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Send interval'),
    '#options' => _notifications_send_intervals(),
    '#default_value' => $subscription->send_interval,
  );
  $send_methods = _notifications_send_methods($account);
  $form['send_method'] = array(
    '#type' => 'select',
    '#title' => t('Send method'),
    '#options' => $send_methods,
    '#default_value' => $subscription->send_method,
    '#disabled' => count($send_methods) < 2,
  );

  $form['confirm'] = array('#type' => 'submit', '#value' => t('Subscribe'));
  $form['cancel'] = array('#type' => 'submit', '#value' => t('Cancel'));
  return $form;
}

/**
 * Process form submission
 */
function notifications_form_subscribe_confirm_submit($form, &$form_state) {
  $subscription = (object)$form_state['values'];
  switch ($form_state['values']['op']) {
    case t('Subscribe'):
      notifications_save_subscription($subscription);
      drupal_set_message(t('Your subscription was activated.'));
      break;
    case t('Cancel'):
      drupal_set_message(t('Your subscription was cancelled'));
      break;
  }
  return 'user/'. $subscription->uid .'/notifications';   
}

/**
 * Process arguments and return an array of field/value pairs
 */
function notifications_field_args($fields, $values) {
  $names = explode(',', $fields);
  $params = explode(',', $values);
  return array_combine($names, $params);  
}

/**
 * Menu callback add subscription
 * 
 * This just admits one field/value pair
 */
function notifications_page_unsubscribe($sid) {
  global $user;
  
  // Check signature if present
  $signed = !empty($_GET['signature']) && $_GET['signature'] == _notifications_signature(array('unsubscribe', $sid), !empty($_GET['confirm']));
  
  if (is_numeric($sid) && ($subscription = notifications_load_subscription($sid)) ) {
    if (user_access('administer notifications') ||
      ($user->uid && $user->uid == $subscription->uid) ||
      $signed
    ) {
      // Skip confirmation page when requested and the signature is ok
      if (!empty($_GET['confirm']) && $signed) {
        notifications_delete_subscription($sid);
        drupal_set_message(t('Your subscription has been removed.'));
        drupal_goto();
      } else {
        drupal_set_title(t('Unsubscribe'));
        return drupal_get_form('notifications_form_unsubscribe_confirm', $subscription);
      }
    }
  }
  drupal_access_denied();
}

/**
 * Form for subscription confirmation
 */
function notifications_form_unsubscribe_confirm($form_state, $subscription) {

  // Pass on subscription values
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);

  // The names will be added here
  notifications_module_invoke('names', $subscription);

  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('!type subscription to', array('!type' => $subscription->type_name)),
    '#value' => theme('item_list', $subscription->names),
  );

  $form['confirm'] = array('#type' => 'submit', '#value' => t('Unsubscribe'));
  $form['cancel'] = array('#type' => 'submit', '#value' => t('Cancel'));
  return $form;
}

/**
 * Process form submission
 */
function notifications_form_unsubscribe_confirm_submit($form, &$form_state) {
  $subscription = $form_state['values']['subscription'];
  switch ($form_state['values']['op']) {
    case t('Unsubscribe'):
      notifications_delete_subscription($subscription->sid);
      drupal_set_message(t('Your subscription has been removed.'));
      break;
    case t('Cancel'):
      // Do nothing, not worth showing a message, just get back
      break;
  }
  return 'user/'. $subscription->uid .'/notifications';   
}