<?php
/**
 * Menu-callback for JavaScript-based uploads.
 */
function yui_editor_image_upload() {
  header("content-type: text/html"); // the return type must be text/html
  $response = null;
  $path = file_directory_path();

  //Append trailing slash to path if not there 
  if (! (substr($path, -1) == '/')) {
    $path .= '/';
  }
  $path .= 'images';
  $file = file_save_upload('upload', array(), $path, FILE_EXISTS_REPLACE);
  if (!$file) {
    $response->status = 'Error Reading Uploaded File.';
    print drupal_to_js($response);
    exit;
  }
  
  $response->status = 'UPLOADED';
  $response->image_url = $file->filepath;

  // Set it to permanent so it doesn't get wiped on the next cron run!
  file_set_status($file, FILE_STATUS_PERMANENT);

  print drupal_to_js($response);
  exit;
}

function yui_editor_img_upload_menu(&$items) {
  $items['yui_editor/image_upload'] = array(
    'page callback' => 'yui_editor_image_upload',
    'access callback' => 'user_access',
    'access arguments' => array('upload files'),
    'type' => MENU_CALLBACK);

  return $items;
}

function yui_editor_img_upload_render(&$profile) {
  $profile['img_upload'] = (user_access('upload files') ? $profile['img_upload'] : 0);
  if ($profile['img_upload'] == 1) {
    drupal_add_js(drupal_get_path("module", "yui_editor")."/plugins/img_upload.js", 'module', 'footer');
  }
}

function yui_editor_img_upload_settings(&$form, &$profile) {
  $form['plugins']['img_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Image upload'),
    '#default_value' => $profile['img_upload'],
    '#description' => t('Allow users to upload images directly from the editor for insertion into the editor. Note: Users must also have \'upload files\' permission set for this functionality to be made available.'));
}
