<?php
//drupal_rebuild_theme_registry();
/**
  * Tendu Drupal 6.x Theme For Developers CSS
  * Updated: Oct 15, 2008 
  * Author: Lior Kesos, Tom Bigelajzen - http://tombigel.com
  * 
  * This theme is Cross-browser and RTL ready.
  * It is extremely minimal and ment to be a wireframe for 
  * theme developers to build bi-directional themes.
  * 
  */

/**
 * CSS Files 
 */ 
drupal_add_css(path_to_theme() .'/style.css', 'theme');
global $language;
if ($language->direction == LANGUAGE_RTL){
  drupal_add_css(path_to_theme() .'/style_rtl.css', 'theme');
}


/**
 * Declare the available regions implemented by this theme.
 *
 * @return
 *   An array of regions.
 */
/**
 * Style Language links (i18n Package)
 * 
 * @return
 *   HTML of the links
 */
function phptemplate_i18n_link($text, $target, $lang, $separator='&nbsp;'){
  $output = '<span class="i18n-link">';
  $attributes = ($lang == i18n_get_lang()) ? array('class' => 'active') : NULL;
  //$output .= l(theme('i18n_language_icon', $lang), $target, $attributes, NULL, NULL, FALSE, TRUE);//Flags
  $output .= $separator;
  // $output .= l($text, $target, $attributes, NULL, NULL, FALSE, TRUE);
  $output .= l(t($text), $target, $attributes, NULL, NULL, FALSE, TRUE);//Text
  $output .= '</span>';
  return $output;
}


/**
 * Override or insert PHPTemplate variables into the page templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 */

function phptemplate_preprocess_page(&$vars) {  // Build array of helpful body classes
  $body_classes = array();  
  $body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';       
                          // Page user is logged in  
  $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front';                
                          // Page is front page
  $body_classes[] = ($vars['node']) ? 'full-node' : '';    

	if ($vars['sidebar_left'] && $vars['sidebar_right']) {
			$body_classes[] = 'two-sidebars';
	} elseif (!$vars['sidebar_left'] && !$vars['sidebar_right']){
			$body_classes[] = 'no-sidebars';
	} else{
			$body_classes[] = 'one-sidebar';
	}

  if ($vars['sidebar_left']) {
    $body_classes[] = 'with-sidebar-left';
  }
  if ($vars['sidebar_right']) {
    $body_classes[] = 'with-sidebar-right';
  }		
  $body_classes = array_filter($body_classes);                                                            // Remove empty elements
  $vars['body_classes'] = implode(' ', $body_classes);                                                    // Create class list separated by spaces
}

?>
