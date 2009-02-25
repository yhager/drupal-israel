<?php

// $Id$

/**
 * Implementation of hook_views_default_views().
 */
function custom_views_default_views() {
  $view = new view;
  $view->name = 'my_content';
  $view->description = 'התכנים האחרונים שהשתתפתי בהם';
  $view->tag = 'default';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
                              'title' => array(
                                'label' => '',
                                'alter' => array(
                                  'alter_text' => 0,
                                  'text' => '',
                                  'make_link' => 0,
                                  'path' => '',
                                  'alt' => '',
                                  'prefix' => '',
                                  'suffix' => '',
                                  'help' => '',
                                  'trim' => 0,
                                  'max_length' => '',
                                  'word_boundary' => 1,
                                  'ellipsis' => 1,
                                  'html' => 0,
                                ),
                                'link_to_node' => 1,
                                'exclude' => 0,
                                'id' => 'title',
                                'table' => 'node',
                                'field' => 'title',
                                'relationship' => 'none',
                              ),
                              'timestamp' => array(
                                'id' => 'timestamp',
                                'table' => 'history_user',
                                'field' => 'timestamp',
                                'label' => '',
                                'comments' => 1,
                                'relationship' => 'none',
                                'link_to_node' => 0,
                                'comment' => 1,
                              ),
                              'edit_node' => array(
                                'label' => '',
                                'alter' => array(
                                  'alter_text' => 0,
                                  'text' => '',
                                  'make_link' => 0,
                                  'path' => '',
                                  'alt' => '',
                                  'prefix' => '',
                                  'suffix' => '',
                                  'help' => '',
                                  'trim' => 0,
                                  'max_length' => '',
                                  'word_boundary' => 1,
                                  'ellipsis' => 1,
                                  'html' => 0,
                                ),
                                'text' => 'עריכה',
                                'exclude' => 0,
                                'id' => 'edit_node',
                                'table' => 'node',
                                'field' => 'edit_node',
                                'override' => array(
                                  'button' => 'Override',
                                ),
                                'relationship' => 'none',
                              ),
                              'view_node' => array(
                                'label' => '',
                                'alter' => array(
                                  'alter_text' => 0,
                                  'text' => '',
                                  'make_link' => 0,
                                  'path' => '',
                                  'alt' => '',
                                  'prefix' => '',
                                  'suffix' => '',
                                  'help' => '',
                                  'trim' => 0,
                                  'max_length' => '',
                                  'word_boundary' => 1,
                                  'ellipsis' => 1,
                                  'html' => 0,
                                ),
                                'text' => 'צפיה',
                                'exclude' => 0,
                                'id' => 'view_node',
                                'table' => 'node',
                                'field' => 'view_node',
                                'override' => array(
                                  'button' => 'Override',
                                ),
                                'relationship' => 'none',
                              ),
                            ));
  $handler->override_option('sorts', array(
                              'last_comment_timestamp' => array(
                                'order' => 'DESC',
                                'granularity' => 'second',
                                'id' => 'last_comment_timestamp',
                                'table' => 'node_comment_statistics',
                                'field' => 'last_comment_timestamp',
                                'relationship' => 'none',
                              ),
                            ));
  $handler->override_option('arguments', array(
                              'uid' => array(
                                'default_action' => 'default',
                                'style_plugin' => 'default_summary',
                                'style_options' => array(),
                                'wildcard' => 'all',
                                'wildcard_substitution' => 'All',
                                'title' => '',
                                'default_argument_type' => 'current_user',
                                'default_argument' => '',
                                'validate_type' => 'none',
                                'validate_fail' => 'empty',
                                'break_phrase' => 0,
                                'not' => 0,
                                'id' => 'uid',
                                'table' => 'users',
                                'field' => 'uid',
                                'relationship' => 'none',
                                'default_options_div_prefix' => '',
                                'default_argument_user' => 0,
                                'default_argument_fixed' => '',
                                'default_argument_php' => '',
                                'validate_argument_node_type' => array(
                                  'blog' => 0,
                                  'poll' => 0,
                                  'forum' => 0,
                                  'bio' => 0,
                                  'book' => 0,
                                  'doil_faq' => 0,
                                  'drupalpro' => 0,
                                  'drupalsite' => 0,
                                  'event' => 0,
                                  'itzuv' => 0,
                                  'page' => 0,
                                  'profile' => 0,
                                  'simplenews' => 0,
                                  'story' => 0,
                                ),
                                'validate_argument_node_access' => 0,
                                'validate_argument_nid_type' => 'nid',
                                'validate_argument_vocabulary' => array(
                                  '1' => 0,
                                  '5' => 0,
                                  '3' => 0,
                                  '7' => 0,
                                  '4' => 0,
                                  '2' => 0,
                                  '6' => 0,
                                ),
                                'validate_argument_type' => 'tid',
                                'user_argument_type' => 'uid',
                                'restrict_user_roles' => 0,
                                'user_roles' => array(),
                                'validate_argument_php' => '',
                              ),
                            ));
  $handler->override_option('filters', array(
                              'type' => array(
                                'operator' => 'in',
                                'value' => array(
                                  'blog' => 'blog',
                                  'forum' => 'forum',
                                  'book' => 'book',
                                  'drupalpro' => 'drupalpro',
                                  'drupalsite' => 'drupalsite',
                                  'event' => 'event',
                                  'page' => 'page',
                                ),
                                'group' => '0',
                                'exposed' => FALSE,
                                'expose' => array(
                                  'operator' => FALSE,
                                  'label' => '',
                                ),
                                'id' => 'type',
                                'table' => 'node',
                                'field' => 'type',
                                'override' => array(
                                  'button' => 'Override',
                                ),
                                'relationship' => 'none',
                              ),
                              'status_extra' => array(
                                'id' => 'status_extra',
                                'table' => 'node',
                                'field' => 'status_extra',
                              ),
                            ));
  $handler->override_option('access', array(
                              'type' => 'none',
                              'role' => array(),
                              'perm' => '',
                            ));
  $handler->override_option('title', 'עדכונים אחרונים');
  $handler->override_option('empty', 'עוד לא השתתפת באתר? נשמח להשתתפותך בקהילה.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_pager', '0');
  $handler->override_option('distinct', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
                              'override' => 1,
                              'order' => 'desc',
                              'columns' => array(
                                'type' => 'type',
                                'title' => 'title',
                                'name' => 'name',
                                'comment_count' => 'comment_count',
                                'last_comment_timestamp' => 'last_comment_timestamp',
                                'timestamp' => 'title',
                                'new_comments' => 'comment_count',
                              ),
                              'info' => array(
                                'type' => array(
                                  'sortable' => 1,
                                  'separator' => '',
                                ),
                                'title' => array(
                                  'sortable' => 1,
                                  'separator' => '&nbsp;',
                                ),
                                'name' => array(
                                  'sortable' => 1,
                                  'separator' => '',
                                ),
                                'comment_count' => array(
                                  'sortable' => 1,
                                  'separator' => '<br />',
                                ),
                                'last_comment_timestamp' => array(
                                  'sortable' => 1,
                                  'separator' => '&nbsp;',
                                ),
                                'timestamp' => array(
                                  'separator' => '',
                                ),
                                'new_comments' => array(
                                  'separator' => '',
                                ),
                              ),
                              'default' => 'last_comment_timestamp',
                            ));
  $handler = $view->new_display('block', 'התכנים שלי', 'block_2');
  $handler->override_option('arguments', array());
  $handler->override_option('title', 'התכנים העדכניים שלך');
  $handler->override_option('block_description', 'התכנים שלי');
  $handler->override_option('block_caching', -1);

  $views[$view->name] = $view;
  return $views;
}