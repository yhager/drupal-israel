<?php
// $Id: site_network.settings.inc,v 1.1.1.1.2.11 2008/07/31 05:35:01 brmassa Exp $
/**
 * @file
 * Module settings.
 */

/**
 * Configure the distributed authentication
 *
 * @ingroup form
 */
function _site_network_settings() {
  $form['client'] = array(
    '#description'    => t('Your Drupal site might accept logins with the user names of other Drupal sites.'),
    '#title'          => t('Client'),
    '#type'           => 'fieldset',
  );
  $form['client']['site_network_client'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Distributed Authentication'),
    '#default_value'  => variable_get('site_network_client', FALSE),
  );
  $form['client']['site_network_client_server'] =  array(
    '#type'           => 'textfield',
    '#title'          => t('Default authentication server'),
    '#default_value'  => variable_get('site_network_client_server', ''),
    '#description'    => t('The URL of the default authentication server. Users registered at this server will not need to append the server to their user name when logging into your site. This enables users to provide a briefer, more familiar username in the login form.'),
  );
  $form['client']['site_network_client_server_only'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Only allow authentication from default server'),
    '#default_value'  => variable_get('site_network_client_server_only', FALSE),
    '#description'    => t('Only accept remote logins from the above specified default authentication server and not from any other server.'),
  );
  $form['client']['site_network_client_notice'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Login notice'),
    '#default_value'  => variable_get('site_network_client_notice', FALSE),
    '#description'    => t('Put a small note bellow the login form warning users that they might use this feature.'),
  );
  $form['server'] = array(
    '#description'    => t('Provide authentication for users logging into other Drupal sites, based on their user accounts here.'),
    '#title'          => t('Server'),
    '#type'           => 'fieldset',
  );
  $form['server']['site_network_server'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Authentication server'),
    '#default_value'  => variable_get('site_network_server', FALSE),
  );

  return system_settings_form($form);
}

/**
 * Validate the settings options
 *
 * @ingroup form
 */
function _site_network_settings_validate(&$form, &$form_state) {
  // Filter the server URL
  $url = trim($form_state['values']['site_network_client_server']);
  if (substr($url, 0, 7) == 'http://') {
    $url = substr($url, 7);
  }
  elseif (substr($url, 0, 8) == 'http://') {
    $url = substr($url, 8);
  }
  form_set_value($form['client']['site_network_client_server'], $url, $form_state);

  // Check if the "force default server" and server are set
  if ($form_state['values']['site_network_client_server_only'] and empty($url)) {
    form_set_error('site_network_client_server', t('You must specify a valid server URL.'));
  }
}
