<?php
// $Id: site_network.authentication.inc,v 1.1.2.6 2008/08/14 23:48:32 brmassa Exp $
/**
 * @file
 * All authentication functions.
 */

/**
 * Validate the user based on another Drupal site
 *
 * @ingroup $form
 */
function _site_network_login_validate(&$form, &$form_state) {
  if (variable_get('site_network_client', FALSE)) {
    // The username must be split in two: the first part is the
    // username itself and the second is the server
    $name     = explode('@', trim($form_state['values']['name']));
    $username = $name[0];

    // If no server was specified, get the default option if its required
    if (empty($name[1]) and variable_get('site_network_client_server', '')) {
      $server = variable_get('site_network_client_server', '');
    }
    else {
      $server = $name[1];
    }

    // Check if the server is the own site
    global $base_url;
    $site_url = explode('://', $base_url);
    if ($server == $site_url[1]) {
      unset($server);
    }

    if (!empty($server) and variable_get('site_network_client_server_only', FALSE)
        and $server != variable_get('site_network_client_server', '')) {
      // If the server is different from the required one, fail
      form_set_error('name', t('Server not allowed'));
    }
    elseif (!empty($server)) {
      // If a server exists, use it
      $aid = xmlrpc("http://$server/xmlrpc.php", 'drupal.login', $username, $form_state['values']['pass']);

      // Log using the external account if the authentication succeded
      if (!empty($aid)) {
        // Put both username and server together again
        $name = "$username@$server";

        // Check if the account already exists or its a new user
        $account = user_external_load($name);
        if (isset($account->uid)) {
          user_external_login($account);
        }
        else {
          user_external_login_register($name, 'site_network');
        }
      }
      elseif (variable_get('site_network_client_server_only', FALSE)) {
        // Disable the other validation processes, since it already failed
        $form_state['#validate'] = array();
        $form_state['#submit']  = array();

        form_set_error('name', t("Sorry, unrecognized username or password."));
      }
    }
    elseif (variable_get('site_network_client_server_only', FALSE)) {
      form_set_error('name', t('Server is required but not set'));
    }
  }
}
