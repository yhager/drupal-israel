<?php
/**
 * Menu-callback for flickr API interaction.
 */
function yui_editor_flickr() {
  header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
  header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
  header("Cache-Control: no-store, no-cache, must-revalidate");
  header("Cache-Control: post-check=0, pre-check=0", false);
  header("Content-Type:text/xml");

  $tags = $_GET['tags'];
  $key = variable_get('yui_editor_flickr_api_key', '');
  $url = "http://www.flickr.com/services/rest/?method=flickr.photos.search&api_key=$key&machine_tag_mode=any&tags=$tags";

  function getResource($url) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $result = curl_exec($ch);
    curl_close($ch);

    return $result;
  }
  
  print getResource($url);
  exit;
}

function yui_editor_flickr_menu(&$items) {
  $items['yui_editor/flickr'] = array(
    'page callback' => 'yui_editor_flickr',
    'access callback' => 'user_access',
    'access arguments' => array('Access YUI editor'),
    'type' => MENU_CALLBACK);
}

function yui_editor_flickr_settings(&$form, &$profile) {
  $form['plugins']['flickr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flickr image insert'),
    '#default_value' => $profile['flickr'],
    '#description' => t('Allows flickr images to be browsed and inserted.'));
  $form['plugins']['flickr_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr API key'),
    '#default_value' => $profile['flickr_api_key'],
    '#size' => 20,
    '#description' => t('Enter your flickr API key if you turned on the \'flickr image insert\' feature.'));
}

function yui_editor_flickr_render(&$profile) {
  if ($profile['flickr'] == 1) {      
    drupal_add_js(drupal_get_path("module", "yui_editor")."/plugins/flickr.js", 'module', 'footer');
    drupal_add_css(drupal_get_path("module", "yui_editor")."/plugins/flickr.css");
  } 
}
