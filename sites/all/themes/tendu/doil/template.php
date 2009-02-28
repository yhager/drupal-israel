<?php
/**
 * Doil: A sub-theme of Tendu - A CSS theme for developers
 * Author: Tom Bigelajzen - http://tombigel.com
 */


/**
 * Put language block in page.tpl
 * 
 * @return 
 * @param $language_switcher: the contents of locale_block['content']
 */
function doil_preprocess_page(&$vars) {
    $lang_switch =  module_invoke('locale', 'block', 'view');
    $vars['language_switcher'] = $lang_switch['content'];
}

function doil_preprocess_node(&$vars) {
  $notifications_links = module_invoke('notifications_ui', 'link', 'node', $vars['node']);
  foreach ($notifications_links as $link) {
    $vars['notifications_links'][] = l($link['title'], $link['href'], $link);
  }
}
