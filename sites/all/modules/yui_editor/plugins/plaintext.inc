<?php
function yui_editor_plaintext_settings(&$form, &$profile) {
  $form['plugins']['plaintext'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plaintext button'),
    '#default_value' => $profile['plaintext'],
    '#description' => t('Allow user to switch between plain text area and YUI editor'));
}

function yui_editor_plaintext_render(&$profile) {
  if ($profile['plaintext'] == 1) {
    drupal_add_js(drupal_get_path("module", "yui_editor")."/plugins/plaintext.js", 'module', 'footer');
  }  
}
