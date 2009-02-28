Drupal.quicktabsShowHide = function() {
  switch (this.value) {
    case 'block':
      $(this).parents('tr')
      .find('td.qt-tab-block-content').show();
      $(this).parents('tr')
      .find('td.qt-tab-node-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-view-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-qtabs-content').hide();
      break;

    case 'node':
      $(this).parents('tr')
      .find('td.qt-tab-block-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-node-content').show();
      $(this).parents('tr')
      .find('td.qt-tab-view-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-qtabs-content').hide();
      break;

    case 'view':
      $(this).parents('tr')
      .find('td.qt-tab-block-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-node-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-view-content').show();
      $(this).parents('tr')
      .find('td.qt-tab-qtabs-content').hide();
      break;

    case 'qtabs':
      $(this).parents('tr')
      .find('td.qt-tab-block-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-node-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-view-content').hide();
      $(this).parents('tr')
      .find('td.qt-tab-qtabs-content').show();
      break;
  }
};

Drupal.behaviors.quicktabsform = function(context) {
  $('#quicktabs-form tr').not('.quicktabs-form-processed').addClass('quicktabs-form-processed').each(function(){
    var currentRow = $(this);
    currentRow.find('div.form-item :input[name*="type"]').bind('click', Drupal.quicktabsShowHide);
    $(':input[name*="type"]:checked', this).trigger('click');
  })
};