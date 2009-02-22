YAHOO.namespace('Drupal');
YAHOO.Drupal.editors = [];
YAHOO.Drupal.yui_editor_load = new YAHOO.util.CustomEvent('load', YAHOO.Drupal.editors);
YAHOO.Drupal.yui_editor = function (id, config) {
  var Dom = YAHOO.util.Dom, Event = YAHOO.util.Event;

  if (Dom.get(id) != null) {
    var newToolbar = new yui_editor_toolbar;
    var myConfig = {
      height: config.height,
      width: config.width,
      dompath: parseInt(config.dom),
      animate: true,
      handleSubmit: true,
      markup: config.markup,
      toolbar: newToolbar.toolbar,
      ptags: parseInt(config.ptags),
    };
    myConfig.toolbar.titlebar = config.title;
    myConfig.toolbar.collapse = parseInt(config.collapse);
    myConfig.toolbar.draggable = parseInt(config.draggable);
    myConfig.toolbar.buttonType = config.buttonType;

    $(document).ready(function() {
      $(".grippie").hide();
      $(".teaser-checkbox").hide();
      YAHOO.util.Dom.setStyle('toggleEditor-' + id, 'margin-top', '1.7em');
    });

    var myEditor = new YAHOO.widget.Editor(id, myConfig);
    if (! config.titlebar) {
      myEditor._defaultToolbar.titlebar = config.titlebar;
    }

    myEditor.render();

    this.editor = myEditor;
    this.config = config;
    this.id = id;
  }
};
