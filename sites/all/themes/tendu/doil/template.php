<?php
// $Id: template.php,v 1.1.2.2 2009/01/19 11:19:30 tombigel Exp $
/**
 * Doil: A sub-theme of Tendu - A CSS theme for developers
 * Author: Tom Bigelajzen - http://tombigel.com
 */

/**
 * Force refresh of theme registry.
 * DEVELOPMENT USE ONLY - COMMENT OUT FOR PRODUCTION
 */
//drupal_rebuild_theme_registry();

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