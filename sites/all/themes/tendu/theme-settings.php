<?php
// $Id: theme-settings.php,v 1.1.2.4 2009/02/22 23:07:50 tombigel Exp $

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
    'toggle_accesibility_links' => 1,
  );

  // Merge the saved variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  // Create the form widgets using Forms API
  $form['toggle_language_switcher'] = array(
    '#type' => 'checkbox',
    '#title' => t('Language Switcher'),
    '#default_value' => $settings['toggle_language_switcher'],
    '#description' => t('Automatically show the Language Switcher at the top of the header when <strong>locale</strong> module is enabled and more then one language is defined for the site.')

  );
  $form['toggle_accesibility_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accessibility links'),
    '#default_value' => $settings['toggle_accesibility_links'],
    '#description' => t('Add links in the header and footer to anchors in the page to ease page navigation for screen reader users and keyboard-only users.<br />In style.css these links are styled as hidden and show when focused <br />(IE does not support ":focus" so in IE browsers it applies only for screen readers or any other no-CSS browsing method)'),
  );
  // Return the additional form widgets
  return $form;
}
?>