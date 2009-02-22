<?php
function yui_editor_resize_settings(&$form, &$profile) {
  $form['plugins']['resize'] = array(
    '#type' => 'checkbox',        
    '#title' => t('Resize'),
    '#default_value' => $profile['resize'],
    '#description' => t('Allow user to resize the editor'));
}

function yui_editor_resize_render(&$profile) {
  if ($profile['resize'] == 1) {
    drupal_add_js(drupal_get_path("module", "yui_editor")."/plugins/resize.js", 'module', 'footer');
  }
}
