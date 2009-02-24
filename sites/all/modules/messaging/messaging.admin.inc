<?php
// $Id: messaging.admin.inc,v 1.1.2.11 2009/01/12 14:14:28 jareyero Exp $
/**
 * @file
 * Messaging Framework - Admin UI
 * 
 * This file includes the settings page and administration for message templates
 * 
 * By Development Seed, http://wwww.developmentseed.org
 */

/**
 * Overview of message parts and sending methods
 */
function messaging_admin_template() {
  $output = '';
  // List message groups
  $groups = module_invoke_all('messaging', 'message groups');
  $rows = array();
  foreach ($groups as $group => $group_info) {
    $rows[] = array(
    l($group_info['name'], 'admin/messaging/template/edit/'.$group),
    !empty($group_info['description']) ? $group_info['description'] : '',
    );
  }
  $output .= theme('box', t('Message groups'), theme('table', NULL, $rows));

  // List sending methods
  $rows = array();
  messaging_method_list();
  foreach (messaging_method_info() as $method => $info) {
    $rows[] = array(
      '<strong>'. $info['name'] .'</strong>',
      !empty($info['description']) ? $info['description'] : ''
    );
  }
  $output .= theme('box', t('Sending methods'), theme('table', NULL, $rows));
  return $output;
}

/**
 * Message groups edit page
 */
function messaging_admin_template_edit($group) {
  $output = '';
  $groups = module_invoke_all('messaging', 'message groups');
  if (isset($groups[$group])) {
    drupal_set_title($groups[$group]['name']);

    $output .= drupal_get_form('messaging_admin_message_form', $group, $groups[$group]);

  }
  return $output;
}

/**
 * Edit message formats
 */
function messaging_admin_message_form($form_state, $group, $group_info) {
  $form['group'] = array('#type' => 'value', '#value' => $group);
  $keylist = module_invoke_all('messaging', 'message keys', $group);
  $send_methods = array('default' => t('Default'));
  $send_methods += messaging_method_list();
  $form['messages'] = array('#tree' => TRUE);
  foreach ($keylist as $key => $keyname) {
    $form['messages'][$key] = array(
      '#type' => 'fieldset',
      '#title' => $keyname,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    foreach ($send_methods as $method => $methodname) {
      $text = messaging_message_part($group, $key, $method, FALSE);
      $form['messages'][$key][$method] = array(
        '#title' => $methodname,
        '#type' => 'textarea',
        '#default_value' => $text,
        // Adjust size to actual number of lines
        '#rows' => count(explode("\n", $text)),        
      );
      
    }
  }
  // Tokens for text replacement
  if ($tokens = messaging_tokens_get_list($group)) {
    $headers = array(t('Token'), t('Replacement value'));
    $rows = array();
    foreach ($tokens as $token => $token_description) {
        $row = array();
        $row[] = '[' . $token . ']';
        $row[] = $token_description;
        $rows[] = $row;
    }
    $form['tokens'] = array(
      '#title' => t('Available tokens'),
      '#type' => 'fieldset',
      '#description' => t('These special strings will be replaced by their real value at run time.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tokens']['list'] = array(
      '#value' => theme('table', $headers, $rows, array('class' => 'description'))
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Get list of tokens for text replacement
 *
 * @param $group
 *   Message group to get tokens for
 * @param $tokens
 *
 */
function messaging_tokens_get_list($group) {
  // First compile token types for this message group
  $type_list = module_invoke_all('messaging', 'tokens', $group);
  // Add known global tokens, will be always available
  $type_list[] = 'global';

  // Now get token list from token module for each type
  $return = array();
  foreach ($type_list as $type) {
    // This is a shortcut for single tokens for digests, with the form (token, description)
    if (is_array($type)) {
      list($type, $token) = $type;
      $list = token_get_list($type);
      $return[$token] = $list[$type][$token];
    }
    elseif ($list = token_get_list($type)) {
      foreach ($list as $category => $tokens) {
        foreach ($tokens as $token => $description) {
          $return[$token] = $description;
        }
      }
    }
  }
  return $return;
}

/**
 * Process and save message parts
 */
function messaging_admin_message_form_submit($form, &$form_state) {
  $group = $form_state['values']['group'];
  foreach ($form_state['values']['messages'] as $key => $messages) {
    foreach ($messages as $method => $text) {
      db_query("DELETE FROM {messaging_message_parts} WHERE type = '%s' AND msgkey = '%s' AND method = '%s'", $group, $key, $method);
      if ($text = trim($text)) {
        db_query("INSERT INTO {messaging_message_parts} (type, msgkey, method, module, message) VALUES('%s', '%s', '%s', '', '%s')", $group, $key, $method, $text);
      }
    }
  }
  drupal_set_message('The messages have been updated');
}

/**
 * Admin settings form
 */
function messaging_admin_settings() {
  // Get plug-in information and produce big warning if none enabled.
  $methods = messaging_method_list();
  if (!$methods) {
    // Get message from requirements
    if ($reqs = messaging_requirements('runtime')) {
      drupal_set_message($reqs['messaging']['value'], 'error');
    }    
  }  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['general']['messaging_default_method'] = array(
    '#title' => t('Default send method'),
    '#type' => 'radios',
    '#options' => $methods,
    '#default_value' => variable_get('messaging_default_method', ''),
  );
  // Logging settings  
  $period = array(0 => t('Disabled')) + drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form['general']['messaging_log'] = array(
    '#title' => t('Logging'),
    '#type' => 'select',
    '#options' => $period,
    '#default_value' => variable_get('messaging_log', 0),
    '#description' => t('If enabled all messages will be logged and kept for the specified time after they\'re sent.'),
  );

  // Sending methods settings
  $form['messaging_methods'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for messaging methods'),
    '#collapsible' => TRUE,
    '#description' => t('Depending on your content format and the tokens you are using for messages it is important that you use the right filtering for the message body.').' '.
      t('Set up the filters you need using the !input_formats page', array('!input_formats' => l('Input Formats', 'admin/settings/filters'))),
  );
  if ($info = messaging_method_info()) {
    foreach (filter_formats() as $format) {
      $format_options[$format->format] = $format->name;
    }
    // We add this last for it not bo be default
    $format_options[0] = t('None (Insecure)');
    foreach ($info as $method => $options) {
      $key = 'messaging_method_'.$method;
      // This will preserve settings for disabled modules
      $form['messaging_methods'][$key] = array(
        '#type' => 'fieldset',
        '#title' => t('!name settings', array('!name' => $options['title'])),
        '#tree' => TRUE,
      );
      // Display name
      $form['messaging_methods'][$key]['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => $options['name'],
        '#description' => t('Name for display.'),
      );      
      // Output filter applied to message body
      $form['messaging_methods'][$key]['filter'] = array(
        '#type' => 'select',
        '#title' => t('Message body filter'),
        '#default_value' => isset($options['filter']) ? $options['filter'] : variable_get('messaging_default_filter', ''),
        '#options' => $format_options,
      );
    }
  } else {
    $form['messaging_methods']['warning'] = array('#value' => t('You should enable some messaging method plug-ins for this to work.'));
  }
  // Processing limits
  $limit = variable_get('messaging_process_limit', array('message' => 0, 'percent' => 0, 'time' => 0));
  $form['messaging_process_limit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Limits for queue processing'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These are the limits for each cron run on queue processing. The process will stop when it first meets any of them. Set to 0 for no limit.'),
  );
  $form['messaging_process_limit']['message'] = array(
    '#title' => t('Number of messages sent'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['message'],
  );
  $form['messaging_process_limit']['time'] = array(
    '#title' => t('Time (seconds)'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['time'],
  );
  $form['messaging_process_limit']['percent'] = array(
    '#title' => t('Time (% of cron time)'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['percent'],
    '#description' => t('Maximum percentage of cron time the process may use.'),
  );
  return system_settings_form($form);
}
