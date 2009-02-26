<?php
// $Id$

// Require the default profile
require_once ('profiles/default/default.profile');
/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function starter_profile_modules() {
  $modules = array(
    // Core Drupal modules.
    'color', 'comment', 'help', 'menu', 'taxonomy', 'dblog', 'path' , 'search', 'contact', 'php',
    // Contributed modules
    'flag', 'messaging', 'messaging_mail', 'messaging_simple', 'notifications', 'nodequeue', // Utilities 1
    'admin_menu', 'mollom', 'token', 'xmlsitemap', // Utilities 2
    'content', 'filefield', 'date', 'imageapi', 'imageapi_gd', 'imagecache', 'imagecache_ui', 'link', 'fieldgroup', 'nodereference', 'number', 'text', 'optionwidgets', 'userreference', // CCK
    'views', 'views_ui', 'advanced_help', // Views
    'codefilter', 'markdown', 'pdir',  // Filters
    'devel', 'coder' // Development
  );

  return $modules;
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function starter_profile_task_list() {
  // This is the only profile method that is invoked before the first page is
  // displayed during the install sequence.  Use this opportunity to theme the
  // install experience.
  global $conf;
  $conf['theme_settings'] = array(
    'default_logo' => 0,
    'logo_path' => 'profiles/starter/linnovate-logo-with-name.gif');
  $conf['site_name'] = 'Linnovate\'s Starter';
   drupal_add_css('profiles/starter/starter.css', 'theme');
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function starter_profile_details() {
  return array(
    'name' => "Starter",
    'description' => "Linnoavte's Starter Kit"
  );
}

function starter_profile_tasks(&$task, $url) {
  // Run the default profile:
  default_profile_tasks($task, $url);
  // Add few more allowed HTML tasks
  $allowed_html_tags = '<a> <p> <br> <em> <strong> <cite> <table> <tr> <td> <th> <tbody> <ul> <ol> <li> <dl> <dt> <dd> <img> <div> <h2> <h3> <h4> <b> <span> <hr> <u>';
  variable_set('allowed_html_1', $allowed_html_tags);
  // Add pdir to both HTML filters:
  db_query("INSERT INTO {filters} (format, module, delta, weight) VALUES (1, 'pdir', 0, 10)"); // Filtered HTML
  db_query("INSERT INTO {filters} (format, module, delta, weight) VALUES (2, 'pdir', 0, 10)"); // Full HTML
  // Add Markdown to both HTML filters:
  db_query("INSERT INTO {filters} (format, module, delta, weight) VALUES (1, 'markdown', 0, -9)"); // Filtered HTML
  db_query("INSERT INTO {filters} (format, module, delta, weight) VALUES (2, 'markdown', 0, -9)"); // Full HTML

  // Prevent anyone from registering:
  variable_set('user_register', '0');

  // Initialize the theme to Tendu:
  db_query("UPDATE {system} SET status = 1 WHERE type='theme' AND name = 'tendu_default'");
  system_theme_data();
  system_initialize_theme_blocks('tendu_default');
  variable_set('theme_default', 'tendu_default');
  $theme_settings = array (
    'toggle_logo' => 1,
    'toggle_name' => 1,
    'toggle_slogan' => 1,
    'toggle_mission' => 1,
    'toggle_node_user_picture' => 0,
    'toggle_comment_user_picture' => 0,
    'toggle_search' => 1,
    'toggle_favicon' => 1,
    'toggle_primary_links' => 1,
    'toggle_secondary_links' => 1,
    'default_logo' => 1,
    'logo_path' => '',
    'logo_upload' => '',
    'default_favicon' => 1,
    'favicon_path' => '',
    'favicon_upload' => '',
    'toggle_language_switcher' => 1,
  );
  variable_set('theme_tendu_default_settings', $theme_settings);
  // Set the footer:
  variable_set('site_footer', 'Starter, By <a title="Drupal made easy" href="http://www.linnovate.net>Linnovate</a>');
}
