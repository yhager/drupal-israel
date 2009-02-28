// $Id: quicktabs.js,v 1.3.2.12 2009/01/13 06:05:01 katbailey Exp $

Drupal.settings.views = Drupal.settings.views || {'ajax_path': 'views/ajax'};

Drupal.behaviors.quicktabs = function (context) {
  $('.quicktabs_wrapper:not(.quicktabs-processed)', context).addClass('quicktabs-processed').each(function(){
    //$(this).prepareQuicktabs();
    Drupal.quicktabs.prepare(this);
  });
};

Drupal.quicktabs = Drupal.quicktabs || {};

// setting up the inital behaviours
Drupal.quicktabs.prepare = function(el) {
  var i = 0;
  $(el).find('ul.quicktabs_tabs li a').each(function(){
    this.myTabIndex = i++;
    $(this).bind('click', quicktabsClick);
  });

  // Search for the active tab.
  var $active_tab = $(el).children('.quicktabs_tabs').find('li.active a');

  if ($active_tab.hasClass('qt_tab') || $active_tab.hasClass('qt_ajax_tab')) {
    $active_tab.trigger('click');
  }
  else {
    // Click on the first tab.
    $(el).children('.quicktabs_tabs').find('li.first a').trigger('click');
  }
  return false;
}

// constructor for an individual tab
Drupal.quicktabs.tab = function (el) {
  this.element = el;
  this.tabIndex = el.myTabIndex;
  this.tabDetails = el.id.split('--');
  this.tabType = this.tabDetails[0];
  this.qtid = this.tabDetails[1];
  var tabKey = this.tabDetails[2];
  this.tabpage_id = 'quicktabs_tabpage_' + this.qtid + '_' + tabKey;
  this.container = $('#quicktabs_container_' + this.qtid);
  this.tabpage = this.container.find('#' + this.tabpage_id);
  // The 'this' variable will not persist inside of the options object.
  var tab = this;
  this.options = {
    success: function(data) {
      return tab.success(data);
    }
  }
}

// ajax callback for non-views tabs
Drupal.quicktabs.tab.prototype.success = function(data) {
  var result = Drupal.parseJson(data);
  //var $container = $('#quicktabs_container_' + tab.qtid);
  this.container.append(Drupal.theme('quicktabsResponse', this, result));
  if (this.tabType == 'qtabs') {
    // The new quicktabs must be prepared.
    var $subQt = this.container.find('#quicktabs-' + this.tabDetails[3]);
    Drupal.quicktabs.prepare($subQt);
  }
  // stop the progress bar
  this.stopProgress();
}


Drupal.quicktabs.tab.prototype.stopProgress = function () {
  if (this.progress.element) {
    $(this.progress.element).remove();
  }
  $(this.element).removeClass('progress-disabled').attr('disabled', false);
}

Drupal.quicktabs.tab.prototype.startProgress = function () {
  var progressBar = new Drupal.progressBar('qt-progress-' + this.element.id, null, null, null);
  progressBar.setProgress(-1, Drupal.t('Loading'));
  this.progress = {};
  this.progress.element = $(progressBar.element).addClass('qt-progress qt-progress-bar');
  //this.progress.object = progressBar;
  this.container.prepend(this.progress.element);
}

Drupal.quicktabs.tab.prototype.quicktabsAjaxView = function() {
  // Create an empty div for the tabpage. The generated view will be inserted into this.
  var tab = this;
  tab.container.append(Drupal.theme('quicktabsResponse', this, null));

  var target;
  target = $('#' + tab.tabpage_id + ' > div');
  var ajax_path = Drupal.settings.views.ajax_path;
   //If there are multiple views this might've ended up showing up multiple times.
  if (ajax_path.constructor.toString().indexOf("Array") != -1) {
    ajax_path = ajax_path[0];
  }
  var args;
  if (tab.tabDetails.length == 6) {
    args = tab.tabDetails[5].replace(/-/g, '/');
  } else {
    args = '';
  }
  var viewData = {
    'view_name': tab.tabDetails[3],
    'view_display_id': tab.tabDetails[4],
    'view_args': args
  }
  $.ajax({
    url: ajax_path,
    type: 'GET',
    data: viewData,
    success: function(response) {
      // Call all callbacks.
      if (response.__callbacks) {
        $.each(response.__callbacks, function(i, callback) {
          eval(callback)(target, response);
        });
      }
      tab.stopProgress();
    },
    error: function() { alert(Drupal.t("An error occurred at @path.", {'@path': ajax_path})); },
    dataType: 'json'
  });
  //break;
}

var quicktabsClick = function() {

  var tab = new Drupal.quicktabs.tab(this);

  // Set clicked tab to active.
  $(this).parents('li').siblings().removeClass('active');
  $(this).parents('li').addClass('active');

  // Hide all tabpages.
  tab.container.children().hide();

  // Show the active tabpage.
  if (tab.tabpage.hasClass('quicktabs_tabpage')) {
    tab.tabpage.show();
  }
  else {
    if ($(this).hasClass('qt_ajax_tab')) {
      tab.startProgress();
      // Construct the ajax tabpage.
      if (tab.tabType != 'view') {
        // construct the ajax path to retrieve the content, depending on type
        var qtAjaxPath = Drupal.settings.basePath + 'quicktabs/ajax/' + tab.tabType + '/' + tab.tabDetails[3];
        if (tab.tabType == 'node') {
          qtAjaxPath +=  '/' + tab.tabDetails[4] + '/' + tab.tabDetails[5];
        }
        if (tab.tabType == 'block') {
          qtAjaxPath +=  '/' + tab.tabDetails[4];
        }
        $.get(qtAjaxPath, null, tab.options.success);
      }
      else {
        // special treatment for views
        tab.quicktabsAjaxView();
      }
    }
  }
  return false;
}

// theme function for ajax response
Drupal.theme.prototype.quicktabsResponse = function(tab, result) {
  var newDiv = tab.tabType == 'view' ? '<div id="' + tab.tabpage_id + '" class="quicktabs_tabpage"><div></div></div>' : '<div id="' + tab.tabpage_id + '" class="quicktabs_tabpage">' + result['data'] + '</div>';
  return newDiv;
}; 
