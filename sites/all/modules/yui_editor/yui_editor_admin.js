YAHOO.namespace('Drupal');
var Dom = YAHOO.util.Dom,
    Event = YAHOO.util.Event;
var DDM = YAHOO.util.DragDropMgr;

editable = {
  config: {
    class_name: 'editable'
  },
  init: function() {
    this.clicked = false;
    this.contents = false;
    this.input = false;

    var _items = Dom.getElementsByClassName(this.config.class_name);
    Event.addListener(_items, 'dblclick', editable.dbl_click, editable, true);
  },
  dbl_click: function(ev) {
    var tar = Event.getTarget(ev);
    if (!tar) {
       return;
     }
     if (tar.tagName && (tar.tagName.toLowerCase() == 'input')) {
       return false;
     }
     this.check();
     this.clicked = tar;
     this.contents = this.clicked.innerHTML;
     this.make_input();
  },
  make_input: function() {
    this.input = Dom.generateId();

    new_input = document.createElement('input');
    new_input.setAttribute('type', 'text');
    new_input.setAttribute('id', this.input);
    if (this.contents == '&nbsp;') {
      new_input.value = '';
    }
    else {
      new_input.value = this.contents;
    }
    new_input.setAttribute('size', this.contents.length);
    new_input.className = 'editable_input';

    this.clicked.innerHTML = '';
    this.clicked.appendChild(new_input);
    new_input.select();
    Event.addListener(new_input, 'blur', editable.check, editable, true);
  },
  clear_input: function() {
    if (this.input) {
      if (Dom.get(this.input).value.length > 0) {
        this.clean_input();
        this.contents_new = Dom.get(this.input).value;
        this.clicked.innerHTML = this.contents_new;
      }
      else {
        this.contents_new = '&nbsp;'
        this.clicked.innerHTML = this.contents_new;
      }
    }
    this.callback();
    this.clicked = false;
    this.contents = false;
    this.input = false;
   },
   clean_input: function() {
     checkText = new String(Dom.get(this.input).value);
     regEx1 = /\"/g;
     checkText = String(checkText.replace(regEx1, ''));
     Dom.get(this.input).value = checkText;
   },
   check: function(ev) {
     if (this.clicked) {
       this.clear_input();
     }
   },
   callback: function() {
 }
}

Event.addListener('edit-button-profile', 'change', function (e) {
  if (Dom.get('edit-button-profile').selectedIndex != (Dom.get('edit-button-profile').length - 1)) {
    var attr = { opacity: { to: 0 } };
    var anim = new YAHOO.util.Anim('custom_button_profile_wrapper', attr, 0.5);
    anim.animate();
    setTimeout("Dom.setStyle(Dom.get('custom_button_profile_wrapper'), 'display', 'none')", 1000);
  }
  else {
    Dom.setStyle(Dom.get('custom_button_profile_wrapper'), 'display', 'block');
    var attr = { opacity: { to: 1 } };
    var anim = new YAHOO.util.Anim('custom_button_profile_wrapper', attr, 0.5);
    anim.animate();
  }
});

Event.addListener('yui-editor-profile', 'submit', function (e) {
  var btn_groups_titles = Dom.get('custom_button_profile').getElementsByTagName('h3');
  var titles = '';
  for (var t = 0; t < (btn_groups_titles.length - 1); t++) {
    titles = titles + "{ group: 'foo', label: \'" + btn_groups_titles[t].innerHTML + "\', buttons: [";
    var g = Dom.getNextSibling(btn_groups_titles[t]).getElementsByTagName('li');
    var btns = '';
    for (var b = 0; b < g.length; b++) {
      var title = Dom.getFirstChild(g[b]).getAttribute('title');
      var value = Dom.getFirstChild(g[b]).getAttribute('value');
      var type = Dom.getFirstChild(g[b]).getAttribute('type');
      btns = btns + (b != 0 ? ',' : '');
      if (title == null) {
        btns = btns + "\n{ type: 'separator' }";
      }
      else if (type == 'spin') {
        var range = Dom.getFirstChild(g[b]).getAttribute('range');
        btns = btns + "\n{ type: '" + type + "', label: '" + title + "', value: '" + value + "', range: " + range + " }";
      }
      else if (type == 'select') {
        var menu = Dom.getFirstChild(g[b]).getAttribute('menu');
        btns = btns + "\n{ type: '" + type + "', label: '" + title + "', value: '" + value + "', menu: " + menu + " }";
      }
      else {
        btns = btns + "\n{ type: '" + type + "', label: '" + title + "', value: '" + value + "' }";
      }
    }
    titles = titles + btns + (t != (btn_groups_titles.length - 1) ? "\n] },\n { type: 'separator' }, \n" : '\n] }\n');
  }
  var btn_groups = Dom.getElementsByClassName('yui-toolbar-group', 'div', Dom.get('custom_button_profile'));
  Dom.getAncestorByTagName(btn_groups[btn_groups.length - 1], 'div').removeChild(Dom.getPreviousSibling(btn_groups[btn_groups.length - 1]));
  Dom.getAncestorByTagName(btn_groups[btn_groups.length - 1], 'div').removeChild(btn_groups[btn_groups.length - 1]);
  Dom.get('edit-custom').value = titles + " { group: 'plugins', label: 'Plugins', buttons: [ ] }";
  Dom.get('edit--custom-html').value = Dom.get('custom_button_profile_wrapper').innerHTML;
});

function yui_editor_add_group() {
  var btn_groups = Dom.getElementsByClassName('yui-toolbar-subcont', 'div', 'custom_button_profile');
  var span = document.createElement('span');
  var div = document.createElement('div');
  var h3 = document.createElement('h3');
  var ul = document.createElement('ul');

  Dom.addClass(span, 'yui-toolbar-separator');
  span.innerHTML = '|';
  Dom.addClass(div, 'yui-toolbar-group');
  h3.innerHTML = '[new group]';
  Dom.addClass(h3, 'editable');
  Event.addListener(h3, 'dblclick', editable.dbl_click, editable, true);
  groupCount++;
  ul.setAttribute('id', 'btngroup' + groupCount);
  Dom.setStyle(ul, 'height', '10px');
  div.appendChild(h3);
  div.appendChild(ul);
  btn_groups[0].appendChild(span);
  btn_groups[0].appendChild(div);
  new YAHOO.util.DDTarget('btngroup' + groupCount);
}

YAHOO.Drupal.DDApp = {
  init: function() {
    var btn_groups = Dom.get('custom_button_profile').getElementsByTagName('ul');
    var bc = 0;
    for (var g = 0; g < btn_groups.length; g++) {
      btn_groups[g].setAttribute('id', 'btngroup' + g);
      new YAHOO.util.DDTarget('btngroup' + g);

      var btns = Dom.get('btngroup' + g).getElementsByTagName('li');
      for (var b = 0; b < btns.length; b++, bc++) {
        btns[b].setAttribute('id', 'btn' + bc);
        new YAHOO.Drupal.DDList('btn' + bc);
      }
    }
    new YAHOO.util.DDTarget('holdingpen');
    var hp = Dom.get('holdingpen').getElementsByTagName('li');
    for (var h = 0; h < hp.length; h++, bc++) {
      hp[h].setAttribute('id', 'btn' + bc);
      new YAHOO.Drupal.DDList('btn' + bc);
    }
    groupCount = g;
    yui_editor_add_group();
  }
};

YAHOO.Drupal.DDList = function(id, sGroup, config) {
  YAHOO.Drupal.DDList.superclass.constructor.call(this, id, sGroup, config);

  this.logger = this.logger || YAHOO;
  var el = this.getDragEl();
  Dom.setStyle(el, "opacity", 0.67);

  this.goingUp = false;
  this.lastY = 0;
};

YAHOO.extend(YAHOO.Drupal.DDList, YAHOO.util.DDProxy, {
  startDrag: function(x, y) {
    var dragEl = this.getDragEl();
    var clickEl = this.getEl();
    Dom.setStyle(clickEl, "visibility", "hidden");

    dragEl.innerHTML = clickEl.innerHTML;

    Dom.setStyle(dragEl, "color", Dom.getStyle(clickEl, "color"));
    Dom.setStyle(dragEl, "backgroundColor", Dom.getStyle(clickEl, "backgroundColor"));
    Dom.setStyle(dragEl, "border", "2px solid gray");
  },
  endDrag: function(e) {
    var srcEl = this.getEl();
    var proxy = this.getDragEl();

    Dom.setStyle(proxy, "visibility", "");
    var a = new YAHOO.util.Motion(proxy, { points: { to: Dom.getXY(srcEl) } }, 0.2, YAHOO.util.Easing.easeOut);
    var proxyid = proxy.id;
    var thisid = this.id;

    a.onComplete.subscribe(function() {
      Dom.setStyle(proxyid, "visibility", "hidden");
      Dom.setStyle(thisid, "visibility", "");
    });
    a.animate();
    var btn_groups = Dom.getElementsByClassName('yui-toolbar-group', 'div', Dom.get('custom_button_profile'));
    for (var g = 0; g < (btn_groups.length - 1); g++) {
      var btns = btn_groups[g].getElementsByTagName('li');
      if (btns.length == 0) {
        if (g != 0) {
          Dom.getAncestorByTagName(btn_groups[g], 'div').removeChild(Dom.getPreviousSibling(btn_groups[g]));
        }
        Dom.getAncestorByTagName(btn_groups[g], 'div').removeChild(btn_groups[g]);
      }
    }
    if (btn_groups[btn_groups.length - 1].getElementsByTagName('li').length > 0) {
      yui_editor_add_group();
    }
  },
  onDragDrop: function(e, id) {
    if (DDM.interactionInfo.drop.length === 1) {
      var pt = DDM.interactionInfo.point; 
      var region = DDM.interactionInfo.sourceRegion; 
      if (!region.intersect(pt)) {
        var destEl = Dom.get(id);
        var destDD = DDM.getDDById(id);
        destEl.appendChild(this.getEl());
        destDD.isEmpty = false;
        DDM.refreshCache();
      }
    }
  },
  onDrag: function(e) {
    var y = Event.getPageY(e);

    if (y < this.lastY) {
      this.goingUp = true;
    }
    else if (y > this.lastY) {
      this.goingUp = false;
    }
    this.lastY = y;
  },
  onDragOver: function(e, id) {
    var srcEl = this.getEl();
    var destEl = Dom.get(id);

    if (destEl.nodeName.toLowerCase() == "li") {
      var orig_p = srcEl.parentNode;
      var p = destEl.parentNode;

      if (this.goingUp) {
        p.insertBefore(srcEl, destEl); // insert above
      }
      else {
        p.insertBefore(srcEl, destEl.nextSibling); // insert below
      }
      DDM.refreshCache();
    }
  }
});

Event.onAvailable('custom_button_profile', editable.init, editable, true);
Event.onContentReady('custom_button_profile', YAHOO.Drupal.DDApp.init, YAHOO.Drupal.DDApp, true);
