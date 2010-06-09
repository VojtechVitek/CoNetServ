/**
 * Object for handling CoNetServ ui
 */
var Ui = {
   /**
    * initializes UI object ( sets tabs, buttons, ... )
    */
   initialize : function() {
      /**
       * Load jquery ui css file depending on selected one from options
       */
      var head = document.getElementsByTagName('head')[0];
      /* JQuery Ui stylesheet */
      $(document.createElement('link'))
      .attr({
         id:'jquery-ui-stylesheet',
         type: 'text/css',
         rel: 'stylesheet',
         media: 'screen'
      })
      .appendTo(head);
      /* Custom CoNetServ stylesheet */
      $(document.createElement('link'))
      .attr({
         id:'conetserv-ui-stylesheet',
         type: 'text/css',
         rel: 'stylesheet',
         media: 'screen'
      })
      .appendTo(head);
      this.reloadSkin();

      /* initialize tabs and its children on selected tab*/
      $("#tabs").tabs();

      /* create radios for virtual tabs */
      $("#local-services-form").buttonset();
      $("#external-services-form").buttonset();
      $("#local-info-form").buttonset();
      $("#external-info-form").buttonset();
      $("#settings-form").buttonset();


      $("#settings-general-frontpage").change(function(){
         Ui.redrawOptions();
      });

      $("#settings-general-submit").button();
      $("#settings-general-submit").click(function() {
         Options.save();
         return false;
      });

      /**
      * reimplement behaviour when different tab is selected
      */
      $('#tabs').tabs({
         show: function(event, ui) {
            // set focus to url-bar
            if(ui.panel.id == "local-services") {
               $("#local-url").focus();
            }
            if(ui.panel.id == "external-services") {
               $("#external-url").focus();
            }
            Ui.redraw();
            return true;
         }
      });

      /**
       * when radiobutton on local-services page is selected
       */
      $("input[name=local-services-form]").change(function(){
         Ui.redraw();
         Plot.repaint();
      });

      /**
       * when radiobutton on external-services page is selected
       */
      $("input[name=external-services-form]").change(function(){
         Ui.redraw();
         Plot.repaint();
      });

      /**
       * when radiobutton on local-info page is selected
       */
      $("input[name=local-info-form]").change(function(){
         Ui.redraw();
         Plot.repaint();
      });

      /**
       * when radiobutton on external-info page is selected
       */
      $("input[name=external-info-form]").change(function(){
         Ui.redraw();
         Plot.repaint();
      });

      /**
       * when radiobutton on settings page is selected
       */
      $("input[name=settings-form]").change(function(){
         Ui.redraw();
      });

      /**
       * style start button
       */
      $("button.url-start").button({
         icons: {
            primary: 'ui-icon-play'
         }
      })
      .removeClass("ui-corner-all")
      .addClass("ui-corner-right");
      

      Ui.redraw();
   },

   /**
    * reloads skin depending on current option
    */
   reloadSkin : function() {
      $("#jquery-ui-stylesheet").attr("href", "css/jquery/" + Options.skin + "/jquery-ui-1.8.2.custom.css")
      $("#conetserv-ui-stylesheet").attr("href", "css/" + Options.skin + ".css")
   },

   /**
   * removes any DOM children in div and instead writes error message by jquery
   * @param div Which divs in DOM should the error should be set to.
   * @param msg Error message.
   */
   divError : function(div, msg) {
      $(div).empty();
      $(div).append(
         '<div class="ui-state-error ui-corner-all" style="padding: 0 .7em;"> \
         <p><span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em;"></span> \
         <strong>Alert:</strong> ' + msg + '</p></div>');
   },

   /**
    * checks availability of services and stuff for current system
    */
   checkAvailability : function() {
      var err;
      /*
       * check for plugin - if not available, disable all dependend parts of
       * CoNetServ
       */
      if(!conetserv) {
         var errStr = "CoNetServ plugin for your browser has not \
            been correctly loaded. Local services will not be available. <br /><br />\
            Please, refer to readme for correct installation steps.";
         this.divError("#local-services", errStr)

         this.divError("#local-info", errStr)
      }
      else {
         /*
          * check local services availability
          * first check for general availability in system - if not, don even display
          */
         return;

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

         // now check for installed state and if not, show message about installation
         if(!conetserv.ping_test) {
            this.divError("#local-ping-div", "Ping service has not been found in your system. \n\
If you want to install it, please follow these steps.");
         }

         if(!conetserv.ping6_test) {
            this.divError("#local-ping6-div", "Ping IPv6 service has not been found in your system. \n\
If you want to install it, please follow these steps.");
         }

         if(!conetserv.tracert_test) {
            this.divError("#local-tracert-div", "Traceroute service has not been found in your system. \n\
If you want to install it, please follow these steps.");
         }

         if(!conetserv.tracert6_test) {
            this.divError("#local-tracert6-div", "Traceroute IPv6 service has not been found in your system. \n\
If you want to install it, please follow these steps.");
         }

         if(!conetserv.whois_test) {
            this.divError("#local-whois-div", "Whois service has not been found in your system. \n\
If you want to install it, please follow these steps.");
         }

         if(!conetserv.nslookup_test) {
            this.divError("#local-nslookup-div", "NSlookup service has not been found in your system. \n\
If you want to install it, please follow these steps.");
         }

         //refresh visibility of buttons
         $("#local-services-form").buttonset("refresh");
      }

   },


   /**
   * Redraws page containing radio buttons used as tabs
   */
   redraw : function() {
      var tabs = $('#tabs').tabs();
      var container = "#" + $("#tabs div.ui-tabs-panel:not(.ui-tabs-hide)").attr("id");

      var active = $(container + " input[type=radio]:checked").val();
      $(container + " .content").css('display', 'none');
      $("#" + active).css('display', 'block');
   },

   /**
   * Shows running state of radio item in menu ( submenu )
   * @param parent Is DOM element, which is parent of input and label items
   * @param selector Defines which radio buttons will be effected
   * @param func Function, which will be called after icon on the right side will
   * be clicked.
   */
   addIcons : function(parent, selector, func) {
      // show icons
      $(parent + " input" + selector).button("option", "icons", {
         secondary:'ui-icon-circle-close'
      });
      // register callback function
      if(func){
         $(parent + " label" + selector + " .ui-button-icon-secondary").click(function(){
            func();
         });
      }
   },
   /**
   * Hides icons of an radio button item in menu ( submenu )
   * @param parent Is DOM element, which is parent of input and label items
   * @param selector Defines which radio buttons will be effected
   */
   removeIcons : function(parent, selector) {
      $(parent + " input" + selector).button("option", "icons", false);
      $(parent + " label" + selector).removeClass("ui-button-text-icon");
   //!!TODO remove empty space after icons are deleted
   },

   /**
   * Redraws options in case they were changed programly
   */
   redrawOptions : function () {
      var parent = $("#settings-general-frontpage input:checked").val();
      $("#settings-general-frontpage-children").html(this.frontpageSettingsChildForm(parent));
   },

   /**
   * Creates form with children
   * @param parent div, which is parenting shown items
   * @return html code to be inserted to a page
   */
   frontpageSettingsChildForm : function (parent) {
      var output = '';
      var label;
      $.each($("#" + parent + " input.radio-tab"), function(index,div) {
         label = $('label[for=' + div.id + ']');
         output += '<div>';
         output += '<input type="radio" id="frontpage-' + div.id + '" class="child" value="' + div.id + '" name="settings-general-frontpage-children"/>';
         output += '<label for="frontpage-' + div.id + '">' + label.text() +'</label>';
         output += '</div>';
      });

      return output;
   }
}

$(function() {
   /**
    * initialize page ui right after page creation
    */
   Ui.initialize();
});
