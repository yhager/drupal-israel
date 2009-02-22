function yui_editor_flickr() {
  for (var e in YAHOO.Drupal.editors) {
    var myEditor = YAHOO.Drupal.editors[e].editor;
    var config = YAHOO.Drupal.editors[e].config;

    if (config.flickr == 1) {
      var gutter = null;
      myEditor.on('toolbarLoaded', function() {
        gutter = new YAHOO.gutter();
        var flickrConfig = {
          type: 'push',
          label: 'Insert Flickr Image',
          value: 'flickr'
        }

        myEditor.toolbar.addButtonToGroup(flickrConfig, 'plugins');

        myEditor.toolbar.on('flickrClick', function(ev) {
          myEditor._focusWindow();
          if (ev && ev.img) {
            var html = '<a href="' + ev.url + '"><img src="' + ev.img + '" title="' + ev.title + '"></a>';
            myEditor.execCommand('inserthtml', html);
          }

          gutter.toggle();
        });
        gutter.createGutter();
      });

      YAHOO.util.Event.onAvailable('flickr_search', function() {
        YAHOO.util.Event.on('flickr_results', 'click', function(ev) {
          var tar = YAHOO.util.Event.getTarget(ev);
          if (tar.tagName.toLowerCase() == 'img') {
            if (tar.getAttribute('fullimage', 2)) {
              var img = tar.getAttribute('fullimage', 2),
                  title = tar.getAttribute('fulltitle'),
                  owner = tar.getAttribute('fullowner'),
                  url = tar.getAttribute('fullurl');
              myEditor.toolbar.fireEvent('flickrClick', {
                type: 'flickrClick',
                img: img,
                title: title,
                owner: owner, 
                url: url
              });
            }
          }
        });

        oACDS = new YAHOO.widget.DS_XHR("",
          ["photo", "title", "id", "owner", "secret", "server"]);
        oACDS.scriptQueryParam = "tags";
        oACDS.responseType = YAHOO.widget.DS_XHR.TYPE_XML;
        oACDS.maxCacheEntries = 0;
        oACDS.scriptQueryAppend = "q=yui_editor/flickr";

        oAutoComp = new YAHOO.widget.AutoComplete('flickr_search','flickr_results', oACDS);
        oAutoComp.autoHighlight = false;
        oAutoComp.alwaysShowContainer = true;     
        oAutoComp.formatResult = function(oResultItem, sQuery) {
          var sTitle = oResultItem[0];
          var sId = oResultItem[1];
          var sOwner = oResultItem[2];
          var sSecret = oResultItem[3];
          var sServer = oResultItem[4];
          var urlPart = 'http:/'+'/static.flickr.com/' + sServer + '/' + sId + '_' + sSecret;
          var sUrl = urlPart + '_s.jpg';
          var lUrl = urlPart + '_m.jpg';
          var fUrl = 'http:/'+'/www.flickr.com/photos/' + sOwner + '/' + sId;
          var sMarkup = '<img src="' + sUrl + '" fullimage="' + lUrl + '" fulltitle="' + sTitle + '" fullid="' +
            sOwner + '" fullurl="' + fUrl + '" class="yui-ac-flickrImg" title="Click to add this image to the editor"><br>';

          return (sMarkup);
        };
      });
    }

    (function() {
      var Dom = YAHOO.util.Dom,
          Event = YAHOO.util.Event;

      YAHOO.gutter = function() {
        return {
          status: false,
          gutter: null,
          createGutter: function() {
            this.gutter = new YAHOO.widget.Overlay('gutter1', {
              height: '425px',
              width: '300px',
              context: [
                myEditor.get('element'),
                'tl',
                'tr'
              ],
              position: 'absolute',
              visible: false
            });
            this.gutter.hideEvent.subscribe(function() {
              myEditor.toolbar.deselectButton('flickr');
              Dom.setStyle('gutter1', 'visibility', 'visible');                
              var anim = new YAHOO.util.Anim('gutter1', {
                width: {
                  from: 300,
                  to: 0
                },
                opacity: {
                  from: 1,
                  to: 0
                }
              }, 1);
              anim.onComplete.subscribe(function() {  
                Dom.setStyle('gutter1', 'visibility', 'hidden');
              });
              anim.animate();
            }, this, true);
            this.gutter.showEvent.subscribe(function() {
              myEditor.toolbar.selectButton('flickr');
              Dom.setStyle(this.gutter.element, 'width', '0px');
              var anim = new YAHOO.util.Anim('gutter1', {
                width: {
                  from: 0,
                  to: 300
                },
                opacity: {
                  from: 0,
                  to: 1
                }
              }, 1);
              anim.animate();
            }, this, true);

            var warn = '';
            if (myEditor.browser.webkit || myEditor.browser.opera) {
              warn = myEditor.STR_IMAGE_COPY;
            }
            this.gutter.setBody('<h2>Flickr Image Search</h2><label for="flikr_search">Tag:</label>' +
              '<input type="text" value="" id="flickr_search">&nbsp;<div id="flickr_results">' + 
              '<p>Enter flickr tags into the box above, separated by commas. Be patient.<p></div>' + warn);
              this.gutter.render(document.body);
            },
            open: function() {
              Dom.get('flickr_search').value = '';
              this.gutter.show();
              this.status = true;
            },
            close: function() {
              this.gutter.hide();
              this.status = false;
            },
            toggle: function() {
              if (this.status) {
                this.close();
              }
              else {
                this.open();
              }
            }
          }
        }
      })
    ();
  }
}
YAHOO.Drupal.yui_editor_load.subscribe(yui_editor_flickr);
