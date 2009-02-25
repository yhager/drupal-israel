<?php
// $Id: template.php,v 1.2.2.6.2.12 2009/02/22 23:07:50 tombigel Exp $
/**
 * Tendu Drupal - A CSS Theme For Developers
 * Author: Tom Bigelajzen (http://drupal.org/user/173787) - http://tombigel.com
 * Initial Drupal 6 porting: 
 *   Lior Kesos (http://drupal.org/user/41517)
 *   Zohar Stolar (http://drupal.org/user/48488) 
 *   http://www.linnovate.net
 */

/* 
 * Force refresh of theme registry.
 * DEVELOPMENT USE ONLY - COMMENT OUT FOR PRODUCTION
 */
//drupal_rebuild_theme_registry();

/*
 * Initialize theme settings
 */
if (is_null(theme_get_setting('toggle_accesibility_links'))) { 
  global $theme_key;
  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(            
    'toggle_language_switcher' => 1,
    'toggle_accesibility_links' => 1,
  );

  // Get default theme settings.
  $settings = theme_get_settings($theme_key);
  // Don't save the toggle_node_info_ variables.
  if (module_exists('node')) {
    foreach (node_get_types() as $type => $name) {
      unset($settings['toggle_node_info_' . $type]);
    }
  }
  // Save default theme settings.
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  // Force refresh of Drupal internals.
  theme_get_setting('', TRUE);
}

function set_language_switcher(){  
  //If there is more then one language defined, add language switcher to page.tpl (defined in theme settings)  
  $lang_switch =  module_invoke('locale', 'block', 'view');
  return '<h2>'.$lang_switch['subject'].'</h2>'.$lang_switch['content'];
}
/**
 * Implement HOOK_theme
 * - Add conditional stylesheets:
 *   For more information: http://msdn.microsoft.com/en-us/library/ms537512.aspx
 */
function tendu_theme(&$existing, $type, $theme, $path){
  
  // Compute the conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    include_once $base_path . drupal_get_path('theme', 'tendu') . '/template.conditional-styles.inc';
    // _conditional_styles_theme() only needs to be run once.
    if ($theme == 'tendu') {
      _conditional_styles_theme($existing, $type, $theme, $path);
    }
  }  
  $templates = drupal_find_theme_functions($existing, array('phptemplate', $theme));
  $templates += drupal_find_theme_templates($existing, '.tpl.php', $path);
  return $templates;
}

/**
 * Override or insert PHPTemplate variables into the page templates.
 * 
 * Note about body classes:
 *  Most of the variables here are Drupals default.  
 *  I changed "page_type" and "node_type" to not add the page/node id to the class,
 *  because I never needed the Drupal classes but I did find a use for a more general page or node type
 *  class, and also added some of my own.
 *  You can change anything here but the dependencies of Tendu's layout must stay intact:
 *  if ($vars['left'] && $vars['right']) {
 *    $body_classes[] = 'two-sidebars';
 *  } elseif (!$vars['left'] && !$vars['right']){
 *    $body_classes[] = 'no-sidebars';
 *  } else{
 *    $body_classes[] = 'one-sidebar';
 *  }
 *  if ($vars['left']) {
 *    $body_classes[] = 'with-sidebar-first';
 *  }
 *  if ($vars['right']) {
 *    $body_classes[] = 'with-sidebar-second';
 *  }  
 *    
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 */

function tendu_preprocess_page(&$vars) {  
  
  //Set Theme Settings dependent variables
  $vars['language_switcher'] = (theme_get_setting('toggle_language_switcher'))? set_language_switcher() : '';
  $vars['accesibility_links'] = (theme_get_setting('toggle_accesibility_links'))? true : false;
      
  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }
  
  // Build array of helpful body classes
  $body_classes = array();
  // Page user is logged in
  $body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';
  // Page is front page
  $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front'; 
  
  //Clean these strings from special characters (TODO: do we need this check?)
  $_page_type = str_replace(array('][', '_', ' '), '-', arg(0));
  $_node_type = str_replace(array('][', '_', ' '), '-', $vars['node']->type);
  // Page type (for admin, node, etc.)
  $body_classes[] = preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-' . $_page_type);
  //If node page, print node type
  if (isset($vars['node']) && $vars['node']->type) {
    $body_classes[] = 'node-type-'. $_node_type;
  }
  
  //Add classes depended on sidebars
  if ($vars['left'] && $vars['right']) {
    $body_classes[] = 'two-sidebars';
  } elseif (!$vars['left'] && !$vars['right']){
    $body_classes[] = 'no-sidebars';
  } else{
    $body_classes[] = 'one-sidebar';
  }

  if ($vars['left']) {
    $body_classes[] = 'with-sidebar-first';
  }
  if ($vars['right']) {
    $body_classes[] = 'with-sidebar-second';
  }
  $body_classes = array_filter($body_classes); // Remove empty elements
  $vars['body_classes'] = implode(' ', $body_classes);// Create class list separated by spaces
}
/**
 * Override block variables to add a variable with "first" and "last" classes to blocks per region
 * Again, most of the variables here are Drupals default.
 * I added $block_region_placement to pass the first/last string to the tpl.
 *  
 * @param $variables
 * A list of block variables
 */
function tendu_preprocess_block(&$variables) {
  
  $block_region_placement = array();  
  static $block_counter = array();
  
  // All blocks get an independent counter for each region.
  if (!isset($block_counter[$variables['block']->region])) {
    $block_counter[$variables['block']->region] = 1;
  }
  
  //Get a list of all blocks in this block's region
  $list = block_list($variables['block']->region);
  //Set class "first" to the first block
  if ($block_counter[$variables['block']->region] == 1) {
     $block_region_placement[] = 'block-first';
  }
  //Set class "last" to the last block
  if ($block_counter[$variables['block']->region] == count($list)) {
     $block_region_placement[] = 'block-last';
  }
  $block_region_placement = array_filter($block_region_placement); // Remove empty elements
  $variables['block_region_placement'] = implode(' ', $block_region_placement);// Create class list separated by spaces
  
  // Continue with Drupal default variables
  $variables['block_zebra'] = ($block_counter[$variables['block']->region] % 2) ? 'odd' : 'even';
  $variables['block_id'] = $block_counter[$variables['block']->region]++;

  $variables['template_files'][] = 'block-'. $variables['block']->region;
  $variables['template_files'][] = 'block-'. $variables['block']->module;
  $variables['template_files'][] = 'block-'. $variables['block']->module .'-'. $variables['block']->delta;
}