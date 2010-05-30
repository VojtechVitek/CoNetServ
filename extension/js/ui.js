

$(function() {
  // before implemented, set values for testing
  var conetserv = {
    ping : 1,
    ping6 : 0,
    tracert : 1,
    tracert6 : 0,
    whois: 1,
    nslookup : 1
  };

  // create tabs
  $("#tabs").tabs();

  //create radios
  $("#local-services-form").buttonset();
  $("#external-services-form").buttonset();
  $("#local-info-form").buttonset();
  $("#external-info-form").buttonset();

  /**
   * checks availability of services and stuff for current system
   */
  var checkAvailability = function() {
    // check for plugin - if not available, disable all dependend parts of
    // CoNetServ
    if(!conetserv) {
      var errstring = "CoNetServ plugin for your browser has not been correctly \
        loaded. Please, refer to readme for further steps."
      $("#local-services").empty();
      // TODO rewrite error call
      $("#local-services").append(
        '<div class="ui-state-error ui-corner-all" style="padding: 0 .7em;"> \
        <p><span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em;"></span> \
        <strong>Alert:</strong> ' + errstring + '</p></div>')
    }

    // check local services availability
    // first check for general availability in system - if not, don even display
    if(!conetserv.ping) {
      $(".local .ping").remove();
    }

    if(!conetserv.ping6) {
      $(".local .ping6").remove();
    }

    if(!conetserv.tracert) {
      $(".local .tracert").remove();
    }

    if(!conetserv.tracert6) {
      $(".local .tracert6").remove();
    }

    if(!conetserv.whois) {
      $(".local .whois").remove();
    }

    if(!conetserv.nslookup) {
      $(".local .nslookup").remove();
    }
    
    //refresh visibility of buttons
    $("#local-services-form").buttonset("refresh");

  };

  /**
   * redraws page containing radio buttons used as tabs
   */
  var redraw = function(){
    var $tabs = $('#tabs').tabs();
    var selected = $tabs.tabs('option', 'selected');
    var container = false;

    //TODO rewrite to better way
    switch(selected) {
      case 0:
        container = '#local-services';
        break;
      case 1:
        container = '#external-services';
        break;
      case 2:
        container = '#local-info';
        break;
      case 3:
        container = '#external-info';
        break;
    }
    var active = $(container + " input:radio:checked").val();
    $(container + " .content").css('display', 'none');
    $("#" + active).css('display', 'block');
  };

  /**
   * on document.ready
   */
  $(document).ready(function(){
    checkAvailability();
    redraw();
  });

  /**
   * reimplement behaviour while different tab is selected
   */
  $('#tabs').tabs({
    show: function(event, ui) {
      redraw();
      return true;
    }
  });
 
  $("input[name=local-services-form]").change(function(){
    redraw();
  });

  
});

