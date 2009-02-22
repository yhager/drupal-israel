function yui_editor_table() {
  for (var e in YAHOO.Drupal.editors) {
    var myEditor = YAHOO.Drupal.editors[e].editor;
    var config = YAHOO.Drupal.editors[e].config;
    var id = YAHOO.Drupal.editors[e].id;

    if (config.table == 1) {
      myEditor.on('toolbarLoaded', function () {
        var tableConfig = {
          type: 'push', label: 'Create Table', value: 'table'
        };
        myEditor.toolbar.addButtonToGroup(tableConfig, 'plugins');
        myEditor.win = null;

        myEditor.createTableHTML = 
          '<table><tbody>' +
          '<tr><td>Rows:</td><td><input type="text" size="1" id="tableRows' + id + '" value="3"></td><td>Columns:</td><td><input type="text" size="1" id="tableColumns' + id + '" value="3"></td></tr>' +
          '<tr><td>Width:</td><td><input type="text" size="1" id="tableWidth' + id + '" value=""></td><td>Height:</td><td><input type="text" size="1" id="tableHeight' + id + '" value=""></td></tr>' +
          '<tr><td><br />Border Size:</td><td colspan="3"><br /><input type="text" size="1" id="tableBorderSize' + id + '" value="1"></td></tr>' +
          '<tr><td>Cell Spacing:</td><td colspan="3"><input type="text" size="1" id="tableCellspacing' + id + '" value="0"></td></tr>' +
          '<tr><td>Cell Padding:</td><td colspan="3"><input type="text" size="1" id="tableCellpadding' + id + '" value="0"></td></tr>' +
          '<tr><td colspan="4"><div align="right"><input type="button" id="tableCreate' + id + '" value="Create"></div></td></tr>' +
          '</tbody></table>';

        myEditor.toolbar.on('tableClick', function(ev) {
          if (this.win !== null) { 
            this.win.setBody(myEditor.createTableHTML); 
            this.win.show(); 
          }
          else { 
            this.win = new YAHOO.widget.Panel('test', { 
              modal: true, 
              fixedcenter: true, 
              draggable: false, 
              width: '350px' 
            }); 
            this.win.setHeader('Create Table'); 
            this.win.setBody(myEditor.createTableHTML); 
            this.win.render(document.body); 
            this.win.hideEvent.subscribe(function() { 
              //Just to make sure we didn't loose it 
              this._setDesignMode('on'); 
              this._focusWindow(); 
            }, myEditor, true);
          } 

          var tableCreateButton = new YAHOO.widget.Button('tableCreate' + id);
          tableCreateButton.on('click', function() {
            var rows =
              document.getElementById('tableRows' + id).value.match('[0-9]+') ? document.getElementById('tableRows' + id).value : 3;
            var columns =
              document.getElementById('tableColumns' + id).value.match('[0-9]+') ? document.getElementById('tableColumns' + id).value : 3;
            var borderSize =
              document.getElementById('tableBorderSize' + id).value.match('[0-9]+') ? document.getElementById('tableBorderSize' + id).value : 1;
            var cellspacing =
              document.getElementById('tableCellspacing' + id).value.match('[0-9]+') ? document.getElementById('tableCellspacing' + id).value : 0;
            var cellpadding =
              document.getElementById('tableCellpadding' + id).value.match('[0-9]+') ? document.getElementById('tableCellpadding' + id).value : 0;
            var width =
              document.getElementById('tableWidth' + id).value.match('[0-9]+') ? document.getElementById('tableWidth' + id).value : 0;
            var height =
              document.getElementById('tableHeight' + id).value.match('[0-9]+') ? document.getElementById('tableHeight' + id).value : 0;
            var tableHTML =
              '<table border="' + borderSize + '" cellspacing="' + cellspacing + '" cellpadding="' + cellpadding + '" height="' + height + '" width="' + width + '"><tbody>';
           
            // Build table
            for (var i = 0;i < rows;i++) {
              tableHTML += '<tr>\n';

              for (var j = 0;j < columns;j++) {
                tableHTML += '<td>&nbsp;</td>\n';
              }

              tableHTML += '</tr>\n';
            }
            tableHTML += '</tbody></table>';
            myEditor.execCommand('inserthtml', tableHTML);
            createTablePanel.hide();
          });

	      return false; 
	    }, myEditor, true); 
      });
    }
  }
}
YAHOO.Drupal.yui_editor_load.subscribe(yui_editor_table);