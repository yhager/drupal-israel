<?php
// $Id: theme-settings.php,v 1.1.2.2 2009/01/25 01:30:44 tombigel Exp $

/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   An array of saved settings for this theme.
 * @return
 *   A form array.
 */
function tendu_settings($saved_settings) {
  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the template.php file.
   */
  $defaults = array(
    'toggle_language_switcher' => 1,
  );

  // Merge the saved variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  // Create the form widgets using Forms API
  $form['toggle_language_switcher'] = array(
    '#type' => 'checkbox',
    '#title' => t('Language Switcher'),
    '#default_value' => $settings['toggle_language_switcher'],
  );
  // Return the additional form widgets
  return $form;
}
?>