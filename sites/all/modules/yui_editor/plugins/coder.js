function yui_editor_coder() {
  for (var e in YAHOO.Drupal.editors) {
    var myEditor = YAHOO.Drupal.editors[e].editor;
    var config = YAHOO.Drupal.editors[e].config;

    var coderState = 'off';
    if (config.coder == 1) {
      myEditor.on('toolbarLoaded', function() {
        var Dom = YAHOO.util.Dom;
        var codeConfig = {
          type: 'push', label: 'Edit HTML Code', value: 'editcode'
        };
        this.toolbar.addButtonToGroup(codeConfig, 'plugins');

        this.toolbar.on('editcodeClick', function(ev) {
          var ta = this.get('element'),
            iframe = this.get('iframe').get('element');

          if (coderState == 'on') {
            coderState = 'off';
            this.toolbar.set('disabled', false);
            this.setEditorHTML(ta.value);
            if (!this.browser.ie) {
              this._setDesignMode('on');
            }

            Dom.removeClass(iframe, 'editor-hidden');
            Dom.addClass(ta, 'editor-hidden');
            this.show();
            this._focusWindow();
          }
          else {
            coderState = 'on';
            this.cleanHTML();
            Dom.addClass(iframe, 'editor-hidden');
            Dom.removeClass(ta, 'editor-hidden');
            this.toolbar.set('disabled', true);
            this.toolbar.getButtonByValue('editcode').set('disabled', false);
            this.toolbar.selectButton('editcode');
            this.dompath.innerHTML = 'Editing HTML Code';
            Dom.setStyle(this.get('element'), 'border', '0px');
            this.hide();
          }

          return false;
        }, this, true);

        this.on('cleanHTML', function(ev) {
          this.get('element').value = ev.html;
        }, this, true);
    
        this.on('afterRender', function() {
          var wrapper = this.get('editor_wrapper');
          wrapper.appendChild(this.get('element'));
          this.setStyle('width', '100%');
          this.setStyle('height', '100%');
          this.setStyle('visibility', '');
          this.setStyle('top', '');
          this.setStyle('left', '');
          this.setStyle('position', '');

          this.addClass('editor-hidden');
        }, this, true);
      }, myEditor, true);
    }

    var toggle = 'off';
    $('#toggleEditor-'+id).bind('click', function () { toggle = 'on'; });
    $('form').bind('submit', function (e) {
      if (toggle == 'on') {
        toggle = 'off';
        return false;
      }
      else if (coderState == 'on') {
        myEditor.setEditorHTML(myEditor.get('textarea').value);
      }
      else {
        myEditor.saveHTML();
      }
    });
  }
}
YAHOO.Drupal.yui_editor_load.subscribe(yui_editor_coder);
