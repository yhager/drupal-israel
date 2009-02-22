function yui_editor_img_upload() {
  for (var e in YAHOO.Drupal.editors) {
    var myEditor = YAHOO.Drupal.editors[e].editor;
    var config = YAHOO.Drupal.editors[e].config;

    if (config.img_upload == 1) {
      yui_img_uploader(myEditor, '?q=/yui_editor/image_upload', 'files[upload]', config.base_path);
    }
  }
}

function yui_img_uploader(rte, upload_url, upload_image_name, base_path) {
  rte.addListener('toolbarLoaded', function() {
    rte.toolbar.addListener('insertimageClick', function(o) {
      try {
        var imgPanel = new YAHOO.util.Element('yui-editor-panel');
        imgPanel.on ('contentReady', function() {
          try {
            var Dom = YAHOO.util.Dom;
            var label = document.createElement('label');
            label.innerHTML = '<strong>Upload:</strong>' +
              '<input type="file" id="insertimage_upload" name="' + upload_image_name +
              '" size="10" style="width: 20%"/>' +
              '<a href="#"  id="insertimage_upload_btn" style="width: 20%; margin-left: 10em;">Upload Image</a>' +
              '</label>';

            var img_elem=Dom.get('insertimage_url');
            Dom.getAncestorByTagName(img_elem, 'form').encoding = 'multipart/form-data';
            Dom.insertAfter(label, img_elem.parentNode);
                                                    
            YAHOO.util.Event.on('insertimage_upload_btn', 'click', function(ev) {
              YAHOO.util.Event.stopEvent(ev); // no default click action
              YAHOO.util.Connect.setForm(img_elem.form, true, true);
              var c = YAHOO.util.Connect.asyncRequest('POST', upload_url, { upload:
			    function(r) {
                  try {
                    resp = r.responseText.replace( /<hints .*/i, '');
                    var o = eval('('+resp+')');
                    if (o.status == 'UPLOADED') {
                      Dom.get('insertimage_upload').value = '';
                      Dom.get('insertimage_url').value=base_path + o.image_url;
                      Dom.get('insertimage_url').focus();
                      Dom.get('insertimage_upload').focus();
                    }
                    else {
                      alert ("Upload Failed: " + o.status);
                    }
                  }
                  catch (eee) {
                    YAHOO.log(eee.message, 'error');
                  }
                }
              });

              return false;
            });
          }
          catch (ee) {
            YAHOO.log(ee.message, 'error');
          }
        });
      }
      catch (e) {
        YAHOO.log(e.message, 'error');
      }
    });
  });
}
YAHOO.Drupal.yui_editor_load.subscribe(yui_editor_img_upload);
