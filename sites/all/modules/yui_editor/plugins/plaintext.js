function yui_editor_plaintext() {
  for (var e in YAHOO.Drupal.editors) {
    var myEditor = YAHOO.Drupal.editors[e].editor;
    var config = YAHOO.Drupal.editors[e].config;
    var id = YAHOO.Drupal.editors[e].id;

    myEditor.plaintextState = 'off';
    if (config.plaintext == 1) {
      $("#"+id).after("<button id=\"toggleEditor-"+id+"\">"+ Drupal.t('Switch to Plain Editor') +"</button>");
      var _button = new YAHOO.widget.Button('toggleEditor-'+id);
      _button.addClass('toggleEditor');

      _button.on('click', function(ev, myEditor) {
        var Event = YAHOO.util.Event;
        var Dom = YAHOO.util.Dom;

        Event.stopEvent(ev);
        if (myEditor.plaintextState == 'off') {
          myEditor.plaintextState = 'on';
          myEditor.saveHTML();
          Dom.setStyle(myEditor.get('element_cont').get('firstChild'), 'position', 'absolute');
          Dom.setStyle(myEditor.get('element_cont').get('firstChild'), 'top', '-9999px');
          Dom.setStyle(myEditor.get('element_cont').get('firstChild'), 'left', '-9999px');
          myEditor.get('element_cont').removeClass('yui-editor-container');
          Dom.setStyle(myEditor.get('element'), 'visibility', 'visible');
          Dom.setStyle(myEditor.get('element'), 'top', '');
          Dom.setStyle(myEditor.get('element'), 'left', '');
          Dom.setStyle(myEditor.get('element'), 'position', 'static');
          _button.set("label", Drupal.t('Switch to Rich Text Editor'));
          $(".grippie").show();
          $(".teaser-checkbox").show();
        }
        else {
          myEditor.plaintextState = 'off';
          Dom.setStyle(myEditor.get('element_cont').get('firstChild'), 'position', 'static');
          Dom.setStyle(myEditor.get('element_cont').get('firstChild'), 'top', '0');
          Dom.setStyle(myEditor.get('element_cont').get('firstChild'), 'left', '0');
          Dom.setStyle(myEditor.get('element'), 'visibility', 'hidden');
          Dom.setStyle(myEditor.get('element'), 'top', '-9999px');
          Dom.setStyle(myEditor.get('element'), 'left', '-9999px');
          Dom.setStyle(myEditor.get('element'), 'position', 'absolute');
          myEditor.get('element_cont').addClass('yui-editor-container');
          myEditor._setDesignMode('on');
          myEditor.setEditorHTML(myEditor.get('textarea').value);
          _button.set("label", Drupal.t('Switch to Plain Editor'));
          $(".grippie").hide();
          $(".teaser-checkbox").hide();
        }
      }, myEditor);
    }

    var toggle = 'off';
    $('#toggleEditor-'+id).bind('click', function () { toggle = 'on'; });
    $('form').bind('submit', function (e) {
      if (toggle == 'on') { 
        toggle = 'off';
        return false;
      }
      else if (myEditor.plaintextState == 'on') {
        myEditor.setEditorHTML(myEditor.get('textarea').value);
      }
      else {
        myEditor.saveHTML();
      }
    });
  }
}
YAHOO.Drupal.yui_editor_load.subscribe(yui_editor_plaintext);
