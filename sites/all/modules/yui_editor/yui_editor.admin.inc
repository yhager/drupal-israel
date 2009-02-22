<?php
/*
 * The profile list page.
 */
function yui_editor_profile_list() {
  $header = array('Priority', 'Name', 'Roles', 'Action');
  $rows = array();
  $profiles = variable_get('yui_editor_profiles', array());
  $roles = user_roles();

  foreach($profiles as $priority => $list) {
    foreach($list as $pid => $profile) {
      $user_roles = '';
      foreach ($profile['roles'] as $rid => $status) {
        if ($status == $rid) {
          $user_roles .= $roles[$rid];
        }
      }
      $rows[] = array($priority, $profile['name'], $user_roles, l('Edit', 'admin/settings/yui_editor/edit/' . $pid) . ' | ' . l('Delete', 'admin/settings/yui_editor/delete/' . $priority . '/' . $pid));
    }
  }

  return theme_table($header, $rows);
}

/*
 * Delete profile.
 */
function yui_editor_profile_delete($priority, $pid) {
  $profiles = variable_get('yui_editor_profiles', array());
  unset($profiles[$priority][$pid]);
  variable_set('yui_editor_profiles', $profiles);

  drupal_set_message(t('Profile deleted'));
  drupal_goto('admin/settings/yui_editor');
}

/*
 * Return a default profile configuration.
 */
function yui_editor_profile_default() {
  return array(
    'name' => 'Default',
    'weight' => 10,
    'roles' => array(2 => 2),
    'include' => '',
    'ids' => '',
    'title' => '',
    'titlebar' => 0,
    'height' => '250px',
    'width' => '100%',
    'markup' => 'xhtml',
    'resize' => 0,
    'plaintext' => 0,
    'coder' => 0,
    'collapse' => 0,
    'draggable' => 0,
    'dom' => 1,
    'img_upload' => 0,
    'flickr' => 0,
    'flickr_api_key' => '',
    'button_type' => 'simple',
    'button_profile' => 'yui_editor_toolbar_default.js');
}

/*
 * The profile add/edit page.
 */
function yui_editor_profile() {
  yui_editor_load_libs();

  if (arg(3) == 'edit' and arg(4)) {
    $profiles = variable_get('yui_editor_profiles', array());
    foreach ($profiles as $priority => $list) {
      foreach ($list as $pid => $profile) {
        if ($pid == arg(4)) {
          $form['old_weight'] = array(
            '#type' => 'hidden',
            '#value' => $profile['weight']);
          $form['id'] = array(
            '#type' => 'hidden',
            '#value' => arg(4));
          break 2;
        }
      }
    }
  }

  if ($profile == null) {
    $profile = yui_editor_profile_default();
    $profile['name'] = '';
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile name'),
    '#required' => TRUE,
    '#default_value' => $profile['name'],
    '#description' => t('The name you want to give this profile'));
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Profile priority and configuration'),
    '#description' => t('Limit where the editor shows up based on role, path, and ID. NOTE: Only ONE profile will be applied per page. If the user has access to the YUI Editor, then the module looks at each profile in order based on the priority. For each profile it compares the user roles and the page path against the profile roles and paths. IDs are NOT used when selecting which profile to apply to a given page, but IDs are useful for specifying which textareas should be applied if the profile is a match (otherwise, it will just place the YUI Editor on the body field -- edit-body).'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE);
  $form['display']['weight'] = array(
    '#type' => 'weight',
    '#title' => 'Priority',
    '#default_value' => $profile['weight'],
    '#description' => t('Priority of this profile in relation to the others (i.e. the weight of this profile)')); 
  $form['display']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Roles',
    '#default_value' => $profile['roles'],
    '#options' => user_roles(),
    '#description' => t('Define which roles YUI should apply this profile. Note: A user only needs to be in one of the roles for this to apply.')); 
  $form['display']['include'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#default_value' => $profile['include'],
    '#description' => t('Define where should YUI replace the body textarea, each path on a new line. You can use wildcards (*). (Leave blank for every form)'),
    '#rows' => 5,
    '#cols' => 60);
  $form['display']['ids'] = array(
    '#type' => 'textarea',
    '#title' => t('IDs'),
    '#default_value' => $profile['ids'],
    '#description' => t('Define which textareas do you want the editor to replace, write the ID of each textarea on a seperate line. (if empty the editor will only replace the body textarea)'),
    '#rows' => 5,
    '#cols' => 20);
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic editor settings'),
    '#description' => t('Basic settings for the editor displayed using this profile'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE);
  $form['basic']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $profile['title'],
    '#description' => t('The title to give the editor'));
  $form['basic']['titlebar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show titlebar'),
    '#default_value' => $profile['titlebar'],
    '#description' => t('Toggle the YUI editor titlebar'));
  $form['basic']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $profile['height'],
    '#description' => t('The height of the editor (i.e. 250px)'));
  $form['basic']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $profile['width'],
    '#description' => t('The width of the editor (i.e. 100%)'));
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced editor settings'),
    '#description' => t('Advanced settings for the editor displayed using this profile'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE);
  $form['advanced']['markup'] = array(
    '#type' => 'textfield',
    '#title' => t('Markup'),
    '#default_value' => $profile['markup'],
    '#description' => t('Set the YUI editor markup'));
  $form['advanced']['collapse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsible titlebar'),
    '#default_value' => $profile['collapse'],
    '#description' => t('Allow user to collapse the titlebar'));
  $form['advanced']['draggable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Draggable titlebar'),
    '#default_value' => $profile['draggable'],
    '#description' => t('Allow user to drag the titlebar'));
  $form['advanced']['dom'] = array(
    '#type' => 'checkbox',
    '#default_value' => $profile['dom'],
    '#title' => t('Show DOM'),
    '#description' => t('Toggle the YUI editor DOM display (at the bottom of the editor)'));
  $form['advanced']['ptags'] = array(
    '#type' => 'checkbox',
    '#default_value' => $profile['ptags'],
    '#title' => t('Use P tags'),
    '#description' => t('Use P tags instead of BR tags for paragraphs. (Shift+enter to get BR tag when this is turned on) Note: This option appears to only work in IE.'));
  $form['plugins'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plugins'),
    '#description' => t('Plugins to apply/enable for editors using this profile'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE);
  $form['buttns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Buttons'),
    '#description' => t('Configure which buttons are applied/enabled for this profile'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE);
  $form['buttns']['button_type'] = array(
    '#type' => 'select',
    '#title' => t('Button type'),
    '#default_value' => $profile['button_type'],
    '#options' => array('simple' => 'Simple', 'advanced' => 'Advanced'),
    '#description' => t('Select the appropriate button profile'));
  $form['buttns']['button_profile'] = array(
    '#type' => 'select',
    '#title' => t('Button profile'),
    '#default_value' => $profile['button_profile'],
    '#options' => array('yui_editor_toolbar_default.js' => 'Default', 'yui_editor_toolbar_no_style.js' => 'No Style Tags', '1'  => 'Custom'),
    '#description' => t('Select the appropriate button profile. If you choose "Custom", then you will be able to build your own toolbar below.'));
  $form['buttns']['_custom_html'] = array(
    '#type' => 'hidden',
    '#default_value' => '');
  $form['buttns']['custom'] = array(
    '#prefix' => ($profile['button_profile'] == 1 ? '<div id="custom_button_profile_wrapper">' . $profile['_custom_html'] . '</div>' : '
      <div id="custom_button_profile_wrapper" style="display: none;">
      <div id="status">You have selected the "Custom" button profile. You may click and drag buttons. You may double click on a button group title and change it.</div>
      <div id="custom_button_profile" class="yui-toolbar-container yui-toolbar-grouped" tabindex="-1"><fieldset>
        <div class="yui-toolbar-subcont">
          <div class="yui-toolbar-group">
            <h3 class="editable">Font Name and Size</h3>
            <ul>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-heading" tabindex="-1" role="button" type="select" value="fontname" unselectable="on" title="Arial" menu="[ { text: \'Arial\', checked: true }, { text: \'Arial Black\' }, { text: \'Comic Sans MS\' }, { text: \'Courier New\' }, { text: \'Lucida Console\' }, { text: \'Tahoma\' }, { text: \'Times New Roman\' }, { text: \'Trebuchet MS\' }, { text: \'Verdana\' } ]">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">Font
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-heading" tabindex="-1" role="button" type="spin" value="fontsize" unselectable="on" title="13" range="[ 9, 75 ]">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">Size
                  </span>
                </span>
              </li>
            </ul>
          </div>
          <span class="yui-toolbar-separator">|</span>
          <div class="yui-toolbar-group">
            <h3 class="editable">Font Style</h3>
            <ul>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-bold" tabindex="-1" role="button" type="push" value="bold" unselectable="on" title="Bold">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-italic" tabindex="-1" role="button" type="push" value="italic" unselectable="on" title="Italic">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-underline" tabindex="-1" role="button" type="push" value="underline" unselectable="on" title="Underline">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-toolbar-separator yui-toolbar-separator-1">|</span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-superscript" tabindex="-1" role="button" type="push" value="superscript" unselectable="on" title="Superscript">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-subscript" tabindex="-1" role="button" type="push" value="subscript" unselectable="on" title="Subscript">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-toolbar-separator yui-toolbar-separator-1">|</span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-forecolor" tabindex="-1" role="button" type="color" value="forecolor" unselectable="on" title="Font color">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-backcolor" tabindex="-1" role="button" type="color" value="backcolor" unselectable="on" title="Background color">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-toolbar-separator yui-toolbar-separator-1">|</span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-removeformat" tabindex="-1" role="button" type="push" value="removeformat" unselectable="on" title="Remove formatting">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-hiddenelements" tabindex="-1" role="button" type="push" value="hiddenelements" unselectable="on" title="Show/Hide hidden elements">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
            </ul>
          </div>
          <span class="yui-toolbar-separator">|</span>
          <div class="yui-toolbar-group">
            <h3 class="editable">Alignment</h3>
            <ul>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-justifyleft" tabindex="-1" role="button" type="push" value="justifyleft" unselectable="on" title="Align left">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-justifycenter" tabindex="-1" role="button" type="push" value="justifycenter" unselectable="on" title="Align center">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-justifyright" tabindex="-1" role="button" type="push" value="justifyright" unselectable="on" title="Align right">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-justifyfull" tabindex="-1" role="button" type="push" value="justifyfull" unselectable="on" title="Justify">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
            </ul>
          </div>
          <span class="yui-toolbar-separator">|</span>
          <div class="yui-toolbar-group">
            <h3 class="editable">Paragraph Style</h3>
            <ul>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-heading" tabindex="-1" role="button" type="select" value="heading" unselectable="on" title="Normal" menu="[ { text: \'Normal\', value: \'none\', checked: true }, { text: \'Header 1\', value: \'h1\' }, { text: \'Header 2\', value: \'h2\' }, { text: \'Header 3\', value: \'h3\' }, { text: \'Header 4\', value: \'h4\' }, { text: \'Header 5\', value: \'h5\' }, { text: \'Header 6\', value: \'h6\' } ]">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">Paragraph
                  </span>
                </span>
              </li>
            </ul>
          </div>
          <span class="yui-toolbar-separator">|</span>
          <div class="yui-toolbar-group">
            <h3 class="editable">Insert Item</h3>
            <ul>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-indent" tabindex="-1" role="button" type="push" value="indent" unselectable="on" title="Indent">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-outdent" tabindex="-1" role="button" type="push" value="outdent" unselectable="on" title="Outdent">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-insertunorderedlist" tabindex="-1" role="button" type="push" value="insertunorderedlist" unselectable="on" title="Create an unordered list">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-insertorderedlist" tabindex="-1" role="button" type="push" value="insertorderedlist" unselectable="on" title="Create an ordered list">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
            </ul>
          </div>
          <span class="yui-toolbar-separator">|</span>
          <div class="yui-toolbar-group">
            <h3 class="editable">Indenting and Lists</h3>
            <ul>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-createlink" tabindex="-1" role="button" type="push" value="createlink" unselectable="on" title="HTML Link">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
              <li class="yui-toolbar-groupitem">
                <span class="yui-button yui-push-button yui-toolbar-insertimage" tabindex="-1" role="button" type="push" value="insertimage" unselectable="on" title="Insert image">
                  <span class="yui-toolbar-icon"/>
                  <span class="first-child">
                  </span>
                </span>
              </li>
            </ul>
          </div>
        </div>
      </fieldset></div>
      <br /><br />
      <div class="yui-toolbar-container yui-toolbar-grouped"><fieldset>
        <div class="yui-toolbar-subcont">
          <div class="yui-toolbar-group">
            <ul id="holdingpen" style="height: 45px;">
              <h3>Removed buttons that are available to be added</h3>
            </ul>
          </div>
        </div>
      </fieldset></div></div>'),
    '#type' => 'hidden');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile'));

  _yui_editor_plugin('settings', $form, $profile);
  drupal_add_js(drupal_get_path("module", "yui_editor")."/yui_editor_admin.js");

  return $form;
}

function yui_editor_profile_submit($form, &$form_state) {
  $profiles = variable_get('yui_editor_profiles', array());

  if (isset($form_state['values']['id'])) {
    drupal_set_message(t('The profile has been updated'));
    unset($profiles[$form_state['values']['old_weight']][$form_state['values']['id']]);
  }
  else {
    drupal_set_message(t('The profile has been created'));
  }
  $form_state['values']['form_token'] = time();
  $profiles[$form_state['values']['weight']][$form_state['values']['form_token']] = $form_state['values'];

  ksort($profiles);
  ksort($profiles[$form_state['values']['weight']]);
  variable_set('yui_editor_profiles', $profiles);

  drupal_goto('admin/settings/yui_editor');
}
