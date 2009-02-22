function yui_editor_resize() {
  for (var e in YAHOO.Drupal.editors) {
    var myEditor = YAHOO.Drupal.editors[e].editor;
    var config = YAHOO.Drupal.editors[e].config;

    if (config.resize == 1) {
      myEditor.on('editorContentLoaded', function() {
        resize = new YAHOO.util.Resize(myEditor.get('element_cont').get('element'), {
          handles: ['br'],
          autoRatio: true,
          status: true,
          proxy: true
        });
        resize.on('startResize', function() {
          this.hide();
          this.set('disabled', true);
        }, myEditor, true);
        resize.on('resize', function(args) {
          var h = args.height;
          var th = (this.toolbar.get('element').clientHeight + 2); //It has a 1px border..
          var dh = (this.dompath.clientHeight + 1); //It has a 1px top border..
          var newH = (h - th - dh);
          this.set('width', args.width + 'px');
          this.set('height', newH + 'px');
          this.set('disabled', false);
          this.show();
        }, myEditor, true);
      });
    }
  }
}
YAHOO.Drupal.yui_editor_load.subscribe(yui_editor_resize);
