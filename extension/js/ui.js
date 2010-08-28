/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/**
 * Object for handling CoNetServ ui
 */
Conetserv.Ui = {
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

      /**
       * Register saving options after changing of forms on options page
       */
      $("#settings-general-frontpage").change(function(){
         Conetserv.Options.save(Conetserv.Options.enums.FRONTPAGE_PARENT);
         Conetserv.Ui.redrawOptions();
      });

      $("#settings-general-frontpage-children").change(function(){
         Conetserv.Options.save(Conetserv.Options.enums.FRONTPAGE_CHILD);
      });

      $("#settings-general-skin").change(function(){
         Conetserv.Options.save(Conetserv.Options.enums.SKIN);
      });

      $("#settings-general-toolbox").change(function(){
         Conetserv.Options.save(Conetserv.Options.enums.TOOLBAR_BUTTON);
      });

      $("#settings-general-autostart").change(function(){
         Conetserv.Options.save(Conetserv.Options.enums.AUTOSTART);
      });

      $("#settings-external-services-form input").change(function(){
         if(!Conetserv.Options.save(Conetserv.Options.enums.EXT_SERVICES))
            $(this).attr("checked", false);
      });

      $("#settings-external-services-form select").change(function(){
         Conetserv.Options.save(Conetserv.Options.enums.EXT_SERVICES_ROUTER);
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
            Conetserv.Ui.redraw();
            return true;
         }
      });

      /**
       * when radiobutton on local-services page is selected
       */
      $("input[name=local-services-form]").change(function(){
         Conetserv.Ui.redraw();
         Conetserv.Plot.repaint();
      });

      /**
       * when radiobutton on external-services page is selected
       */
      $("input[name=external-services-form]").change(function(){
         Conetserv.Ui.redraw();
         Conetserv.Plot.repaint();
      });

      /**
       * when radiobutton on local-info page is selected
       */
      $("input[name=local-info-form]").change(function(){
         Conetserv.Ui.redraw();
         Conetserv.Plot.repaint();
      });

      /**
       * when radiobutton on external-info page is selected
       */
      $("input[name=external-info-form]").change(function(){
         Conetserv.Ui.redraw();
         Conetserv.Plot.repaint();
      });

      /**
       * when radiobutton on settings page is selected
       */
      $("input[name=settings-form]").change(function(){
         Conetserv.Ui.redraw();
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

      /**
       * style alert dialog
       */
      $('#dialog').dialog({
			autoOpen: false,
			show: 'blind',
			modal: true,
         buttons: {
				Ok: function() {
					$(this).dialog('close');
				}
			}
		});

      Conetserv.Ui.redraw();
   },

   /**
    * reloads skin depending on current option
    */
   reloadSkin : function() {
      $("#jquery-ui-stylesheet").attr("href", "css/jquery/" + Conetserv.Options.skin + "/jquery-ui-1.8.2.custom.css")
      $("#conetserv-ui-stylesheet").attr("href", "css/" + Conetserv.Options.skin + ".css")
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
    * Adds installation steps to div and formats mesage depending on current operating system
    * @param div Which div in DOM will be the message appended to
    * @param service name of service to be appended to commands
    */
   divInstallationSteps : function(div, service) {
      var output =  '<div class="ui-state-highlight ui-corner-all" style="padding: 0.7em; margin-top:10px;"> \
         <p><span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>';
      if($.client.os == "Windows") {
         output +=
         '';
      }
      else if($.client.os == "Linux") {
         output += '<strong>For installation please run following command depending on your linux distribution:</strong><br/><br/> \
            Ubuntu/Debian: $ sudo apt-get install ' + service + '<br/> \
            SUSE/OpenSUSE: $ sudo zypper install ' + service + '<br/> \
            Fedora/Yellow Dog Linux: $ sudo yum install ' + service + '<br/> \
            RedHat/CentOS: $ up2date ' + service + '<br/> \
            Gentoo: $ emerge ' + service + '<br/> \
            Mandriva: $ urpmi ' + service + '<br/> \
            <br/>\
            If your distribution is not listed, use your package installation program instead of listed ones.';
      }
      else if($.client.os == "Mac") {

      }

      output += '</p></div>';
      $(div).append(output);
   },

   /**
    * checks availability of services and stuff for current system
    */
   checkAvailability : function() {
      /*
       * check for plugin - if not available, disable all dependend parts of
       * CoNetServ
       */
      if(!Conetserv.plugin) {
         var errStr = "CoNetServ plugin for your browser has not \
            been correctly loaded. <br /> <br/>Local services and local info will not be available. <br /><br />\
            Please, refer to readme for correct installation steps.";

         Conetserv.LocalServices.enabled = false;
         this.divError("#local-services", errStr);
         this.divError("#local-info", errStr);
         return;
      }

      /* Check plugin version */
      var version = Conetserv.plugin.version ? Conetserv.plugin.version.split('.') : false;

      if(parseInt(version[0]) != 1 || parseInt(version[1]) != 9) {
         var errStr = "<strong>CoNetServ extension is incompatible with currently installed plugin.</strong><br/><br/>\
            There might be severe problems with functionality - please reinstall your CoNetServ extension."

         Conetserv.LocalServices.enabled = false;
         Conetserv.ExternalServices.enabled = false;
         this.divError("#local-services", errStr);
         this.divError("#local-info", errStr);
         return;
      }
      /*
       * check local services availability
       * first check for general availability in system - if not, don even display
       */

      if(!Conetserv.plugin.ping) {
         $(".local .ping").remove();
      }
      else {
         if(!Conetserv.plugin.ping.found) {
            this.divError("#local-ping-div", "Ping service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-ping-div", "ping");
         }
      }

      if(!Conetserv.plugin.ping6) {
         $(".local .ping6").remove();
      }
      else {
         if(!Conetserv.plugin.ping6.found) {
            this.divError("#local-ping6-div", "Ping IPv6 service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-ping6-div", "ping6");
         }
      }


      if(!Conetserv.plugin.tracert) {
         $(".local .tracert").remove();
      }
      else {
         if(!Conetserv.plugin.tracert.found) {
            this.divError("#local-tracert-div", "Traceroute service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-tracert-div", "tracert");
         }
      }

      if(!Conetserv.plugin.tracert6) {
         $(".local .tracert6").remove();
      }
      else {
         if(!Conetserv.plugin.tracert6.found) {
            this.divError("#local-tracert6-div", "Traceroute IPv6 service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-tracert6-div", "tracert6");
         }
      }

      if(!Conetserv.plugin.whois) {
         $(".local .whois").remove();
      }
      else {
         if(!Conetserv.plugin.whois.found) {
            this.divError("#local-whois-div", "Whois service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-whois-div", "whois");
         }
      }

      if(!Conetserv.plugin.nslookup) {
         $(".local .nslookup").remove();
      }
      else {
         if(!Conetserv.plugin.nslookup.found) {
            this.divError("#local-nslookup-div", "Nslookup service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-nslookup-div", "nslookup");
         }
      }

      if(!Conetserv.plugin.nmap) {
         $(".local .nmap").remove();
      }
      else {
         if(!Conetserv.plugin.nmap.found) {
            this.divError("#local-nmap-div", "Nmap service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-nmap-div", "nmap");
         }
      }

      if(!Conetserv.plugin.dig) {
         $(".local .dig").remove();
      }
      else {
         if(!Conetserv.plugin.nslookup.found) {
            this.divError("#local-dig-div", "Dig service has not been found in your system. \n\
            If you want to install it, please follow these steps.");
            this.divInstallationSteps("#local-dig-div", "nslookup");
         }
      }

      //refresh visibility of buttons
      $("#local-services-form").buttonset("refresh");

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
   * @param arg Argument to be passed to the function
   * be clicked.
   */
   addIcons : function(parent, selector, func, arg, icon) {
      // show icons
      $(parent + " input" + selector).button("option", "icons", {
         secondary: icon
      });
      // register callback function
      if(func){
         $(parent + " label" + selector + " .ui-button-icon-secondary").click(function(){func(arg);});
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
