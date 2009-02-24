<?php
// $Id: theme-settings.php,v 1.1.2.1 2009/01/24 18:31:51 tombigel Exp $

// Include the definition of tendu_settings().
include_once $base_path . drupal_get_path('theme', 'tendu') . '/theme-settings.php';

function tendu_default_settings($saved_settings){
    return tendu_settings($saved_settings);
}
?>
