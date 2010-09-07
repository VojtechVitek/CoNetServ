/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/**
 * Object for manipulating CoNetServ options
 */
Conetserv.Options = {
   storage : false,
   toolbarButton : function() {
      return this._getBoolean('conetserv-settings-general-toolbox');
   },
   autostart : function() {
      return this._getBoolean('conetserv-settings-general-autostart');
   },
   frontPageParent : function() {
      return this._getString('conetserv-settings-general-frontpage', false);
   },
   frontPageChild : function() {
      return this._getString('conetserv-settings-general-frontpage-child', false);
   },
   skin: function() {
      return this._getString('conetserv-settings-general-skin', $("#settings-general-skin input").val());
   },
   ext_services: function() {
      return this._getArray("conetserv-settings-external-services",
         "true;true;;true;;;;;;;true;true;;;;;;;;;true;;;true;;;;;;;true;true".split(";"));
   },
   ext_services_router: function() {
      return this._getArray("conetserv-settings-external-services_router",
         "r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp;;;;;;;;;r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp".split(";"));
   },
   version: function() {
      return this.storage["conetserv-version"];
   },

   LocalServices: {
      ping_packet_count : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-ping-packet-count");
      },
      ping_timeout : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-ping-timeout");
      },
      ping_ttl : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-ping-ttl");
      },
      ping_packet_size : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-ping-packet-size");
      },
      ping_plot_count : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-ping-plot-count");
      },
      ping_console_unlimited : function() {
         return Conetserv.Options._getBoolean("conetserv-settings-local-services-plot-console-unlimited");
      },

      tracert_max_hops : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-tracert-max-hops");
      },
      tracert_wait_time : function() {
         return Conetserv.Options._getNumeric("conetserv-settings-local-services-tracert-wait-time");
      },
      tracert_ip_to_hostname : function() {
         return Conetserv.Options._getBoolean("conetserv-settings-local-services-tracert-ip-to-hostname");
      }
   },

   //enums for easier argument passing to functions
   enums : {
      TOOLBAR_BUTTON :        0,
      AUTOSTART :             1,
      FRONTPAGE_PARENT :      2,
      FRONTPAGE_CHILD :       3,
      SKIN :                  4,
      EXT_SERVICES :          5,
      EXT_SERVICES_ROUTER :   6,
      LOC_SERVICES_PING :     7,
      LOC_SERVICES_TRACERT :  8,
   },

   /**
    * initializes CoNetServ options
    */
   initialize : function() {
      // fix localStorage for firefox
      if($.client.browser == "Firefox") {
         try {
            this.storage = globalStorage["conetserv"];
         } catch(err) {
            this.storage = false;
         }
      }
      else if($.client.browser == "Chrome") {
         this.storage = localStorage;
      }
      else {
         this.storage = false;
      }

      if(!this.storage) {
         Conetserv.Ui.divError("#settings", "<strong>Local storage initialization has failed.\
            </strong> <br /><br />\
            Please make sure you are using supported web browser.");
         return;
      }

      var reinitialize = false;

      /* Conetserv is run for the first time */
      if(!this.version() || this.version().toString() == '') {
         reinitialize = true;
         if(Conetserv.LocalServices.enabled)
            Conetserv.LocalServices.Ping.console.setErr('<strong>Welcome to CoNetServ extension.</strong><br />Settings has been set to default values.');
      }
      /* Conetserv has been updated */
      else if(this.version() != Conetserv.version){
         setTimeout('Conetserv.Ui.showDialog("CoNetServ has been updated to version " + Conetserv.version, "Release notes: <br/>" + Conetserv.changelog)', 100);
         /* check, if mayor version change has occured */
         if(this.version().toString().substr(0, 1) != Conetserv.version.substr(0,1)) {
            reinitialize = true;
         }
         /* Otherwise just set new version */
         else {
            this.storage['conetserv-version'] = Conetserv.version;
         }
      }

      if(reinitialize) {
         this.storage['conetserv-version'] = Conetserv.version;

         this.storage['conetserv-settings-general-toolbox'] = false;
         this.storage['conetserv-settings-general-autostart'] = false;

         this.storage['conetserv-settings-general-frontpage'] = "local-services";
         this.storage['conetserv-settings-general-frontpage-child'] = "local-ping-input";

         this.storage['conetserv-settings-general-skin'] = $("#settings-general-skin input").val();

         this.storage["conetserv-settings-external-services"] = "true;true;;true;;;;;;;true;true;;;;;;;;;true;;;true;;;;;;;true;true";
         this.storage["conetserv-settings-external-services_router"] = "r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp;;;;;;;;;r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp";
      }


      $("#settings-general-toolbox").attr("checked", this.toolbarButton());

      $("#settings-general-autostart").attr("checked", this.autostart());

      $("#frontpage-" + this.frontPageParent()).attr("checked", true);
      Conetserv.Ui.redrawOptions();
      $("#frontpage-" + this.frontPageChild()).attr("checked", true);

      /* depending on options page, set the active tab */
      $("#tabs").tabs('select', "#" + this.frontPageParent());
      $("#" + this.frontPageParent()).ready(function() {
         $("#" + Conetserv.Options.frontPageChild()).attr("checked", true);
      });

      /* skin options */
      $("#settings-general-skin input[value="+this.skin()+"]").attr("checked", true);

      /* set buttons checked on external services page */
      for(var key in this.ext_services()) {
         if(this.ext_services()[key]) {
            $("#settings-external-services-form input[value="+key+"]").attr("checked", true);
         }
      }
      /* external services router */
      var ext_services_router = this.ext_services_router();
      for(key in ext_services_router) {
         if(ext_services_router[key]) {
            switch(parseInt(key)) {
               case Conetserv.LookingGlass.enums.CERN_PING_V4:
                  $("#settings-external-services-cern-ipv4-router").attr("value", ext_services_router[key]);
                  break;
               case Conetserv.LookingGlass.enums.SPARKLE_PING_V4:
                  $("#settings-external-services-sparkle-ipv4-router").attr("value", ext_services_router[key]);
                  break;
               case Conetserv.LookingGlass.enums.ATMAN_PING_V4:
                  $("#settings-external-services-atman-ipv4-router").attr("value", ext_services_router[key]);
                  break;
               case Conetserv.LookingGlass.enums.ILAN_PING_V4:
                  $("#settings-external-services-ilan-ipv4-router").attr("value", ext_services_router[key]);
                  break;
              case Conetserv.LookingGlass.enums.CERN_PING_V6:
                  $("#settings-external-services-cern-ipv6-router").attr("value", ext_services_router[key]);
                  break;
              case Conetserv.LookingGlass.enums.ATMAN_PING_V4:
                  $("#settings-external-services-atman-ipv6-router").attr("value", ext_services_router[key]);
                  break;
            }
         }
      }

      /*************************************************************************
       *                             LOCAL SERVICES                            *
       ************************************************************************/
      /* ping */
      if(this.LocalServices.ping_packet_count()){
         $("#settings-local-services-ping-count").val(this.LocalServices.ping_packet_count());
      }
      if(this.LocalServices.ping_timeout()){
         $("#settings-local-services-ping-timeout").val(this.LocalServices.ping_timeout());
      }
      if(this.LocalServices.ping_ttl()){
         $("#settings-local-services-ping-ttl").val(this.LocalServices.ping_ttl());
      }
      if(this.LocalServices.ping_packet_size()){
         $("#settings-local-services-ping-packet-size").val(this.LocalServices.ping_packet_size());
      }
      if(this.LocalServices.ping_plot_count()){
         $("#settings-local-services-ping-plot-count").val(this.LocalServices.ping_plot_count());
      }
      $("#settings-local-services-ping-console-unlimited").attr("checked", this.LocalServices.ping_console_unlimited());

      /* traceroute*/
      if(this.LocalServices.tracert_max_hops()){
         $("#settings-local-services-tracert-max-hops").val(this.LocalServices.tracert_max_hops());
      }

      if(this.LocalServices.tracert_wait_time()){
         $("#settings-local-services-tracert-wait-time").val(this.LocalServices.tracert_wait_time());
      }

      $("#settings-local-services-tracert-ip-to-hostname").attr("checked", this.LocalServices.tracert_ip_to_hostname());

   },

   /**
    * Saves options, depending on set values in forms.
    */
   save : function(option) {
      if(!this.storage)
         return false;
      switch(option) {
         case this.enums.TOOLBAR_BUTTON:
            this.storage["conetserv-settings-general-toolbox"] = $("#settings-general-toolbox").is(":checked");
            break;
         case this.enums.AUTOSTART:
            this.storage["conetserv-settings-general-autostart"] = $("#settings-general-autostart").is(":checked");
            break;
         case this.enums.FRONTPAGE_PARENT:
            this.storage["conetserv-settings-general-frontpage"] = $("#settings-general-frontpage input:checked").val();
            break;
         case this.enums.FRONTPAGE_CHILD:
            this.storage["conetserv-settings-general-frontpage-child"] = $("#settings-general-frontpage-children input:checked").val();
            break;
         case this.enums.SKIN:
            this.storage["conetserv-settings-general-skin"] = $("#settings-general-skin input:checked").val();
            Conetserv.Ui.reloadSkin();
            break;
         case this.enums.EXT_SERVICES:
            /* clean ext_services and fill them depending on checked services */
            var ext_services = [];

            /* variables for storing number of active services */
            var ping = 0;
            var ping6 = 0;
            var tracert = 0;
            var tracert6 = 0;

            $("#settings-external-services-form input:checked").each(function(){
               /* check for type of service and check maximum allowed count of running services */
               switch($(this).attr("class")){
                  case "ping":
                     ++ping;
                     break;
                  case "ping6":
                     ++ping6;
                     break;
                  case "tracert":
                     ++tracert;
                     break;
                  case "tracert6":
                     ++tracert6;
                     break;
               }
               ext_services[$(this).attr("value")] = true;
            });

            /* if maximum alowed services has already been reached, return false */
            if(ping > 3 || ping6 > 3 || tracert > 2 || tracert6 > 2) {
               Conetserv.Ui.showDialog("Warning!", "You can allow only 3 ping services and 2 traceroute services in both IPv4 and IPv6.");
               return false;
            }

            /* join array and save to storage */
            this.storage["conetserv-settings-external-services"] = ext_services.join(";");
            break;
         case this.enums.EXT_SERVICES_ROUTER:
            var ext_services_router = this.ext_services_router();

            $("#settings-external-services-form select").each(function(){
               switch($(this).attr("id")) {
                  case "settings-external-services-cern-ipv4-router":
                     ext_services_router[Conetserv.LookingGlass.enums.CERN_PING_V4] = $(this).attr("value");
                     ext_services_router[Conetserv.LookingGlass.enums.CERN_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-sparkle-ipv4-router":
                     ext_services_router[Conetserv.LookingGlass.enums.SPARKLE_PING_V4] = $(this).attr("value");
                     ext_services_router[Conetserv.LookingGlass.enums.SPARKLE_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-atman-ipv4-router":
                     ext_services_router[Conetserv.LookingGlass.enums.ATMAN_PING_V4] = $(this).attr("value");
                     ext_services_router[Conetserv.LookingGlass.enums.ATMAN_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-ilan-ipv4-router":
                     ext_services_router[Conetserv.LookingGlass.enums.ILAN_PING_V4] = $(this).attr("value");
                     ext_services_router[Conetserv.LookingGlass.enums.ILAN_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-cern-ipv6-router":
                     ext_services_router[Conetserv.LookingGlass.enums.CERN_PING_V6] = $(this).attr("value");
                     ext_services_router[Conetserv.LookingGlass.enums.CERN_TRACERT_V6] = $(this).attr("value");
                     break;
                  case "settings-external-services-atman-ipv6-router":
                     ext_services_router[Conetserv.LookingGlass.enums.ATMAN_PING_V6] = $(this).attr("value");
                     ext_services_router[Conetserv.LookingGlass.enums.ATMAN_TRACERT_V6] = $(this).attr("value");
                     break;
               }
            });
            this.storage["conetserv-settings-external-services_router"] = ext_services_router.join(";");
            break;

         case this.enums.LOC_SERVICES_PING:
            if(!this._saveNumeric("conetserv-settings-local-services-ping-packet-count", $("#settings-local-services-ping-count").val())) {
               Conetserv.Ui.showDialog("Warning!", "Number of packets can only be set by a positive numeric value.");
               return false;
            }
            if(!this._saveNumeric("conetserv-settings-local-services-ping-timeout", $("#settings-local-services-ping-timeout").val())) {
               Conetserv.Ui.showDialog("Warning!", "Timeout can only be set by a positive numeric value.");
               return false;
            }
            if(!this._saveNumeric("conetserv-settings-local-services-ping-ttl", $("#settings-local-services-ping-ttl").val())) {
               Conetserv.Ui.showDialog("Warning!", "Packet's time to live can only be set by a positive numeric value.");
               return false;
            }
            if(!this._saveNumeric("conetserv-settings-local-services-ping-packet-size", $("#settings-local-services-ping-packet-size").val())) {
               Conetserv.Ui.showDialog("Warning!", "Packet size to live can only be set by a positive numeric value.");
               return false;
            }
            if(!this._saveNumeric("conetserv-settings-local-services-ping-plot-count", $("#settings-local-services-ping-plot-count").val())) {
               Conetserv.Ui.showDialog("Warning!", "Number of values in plot can only be set by a positive numeric value.");
               return false;
            }
            else {
               if(this.LocalServices.ping_plot_count() < 10 || this.LocalServices.ping_plot_count() > 100) {
                  this._saveNumeric("conetserv-settings-local-services-ping-plot-count", 30)
                  $("#settings-local-services-ping-plot-count").val(30);
                  Conetserv.Ui.showDialog("Warning!", "Number of values to be shown in ping plot must be between 10 and 100.");
               }
            }
            this._saveBoolean("conetserv-settings-local-services-plot-console-unlimited", $("#settings-local-services-ping-console-unlimited").is(":checked"));
            break;

         case this.enums.LOC_SERVICES_TRACERT:
            if(!this._saveNumeric("conetserv-settings-local-services-tracert-max-hops", $("#settings-local-services-tracert-max-hops").val())) {
               Conetserv.Ui.showDialog("Warning!", "Maximum number of hops can only be set by a positive numeric value.");
               return false;
            }

            if(!this._saveNumeric("conetserv-settings-local-services-tracert-wait-time", $("#settings-local-services-tracert-wait-time").val())) {
               Conetserv.Ui.showDialog("Warning!", "Wait time can only be set by a positive numeric value.");
               return false;
            }

            this._saveBoolean("conetserv-settings-local-services-tracert-ip-to-hostname", $("#settings-local-services-tracert-ip-to-hostname").is(":checked"));

            break;
      }

      return true;
   },

   /**
    * Private function for saving numeric data to localstorage
    * empty value is valid and returns true
    * @param id local storage id
    * @param val value to be stored
    * @return true on success, false on failure
    */
   _saveNumeric : function(id, val) {
      if(!parseInt(val) && val != "") {
         this.storage[id] = false;
         return false;
      }
      this.storage[id] = val;
      return true;
   },

   /**
    * Private function for saving boolean data to localstorage
    * empty value is valid and returns true
    * @param id local storage id
    * @param val value to be stored
    * @return true on success, false on failure
    */
   _saveBoolean : function(id, val) {
      this.storage[id] = val;
      return true;
   },

   /**
    * Private function for loading numeric data from localstorage
    * @param id local storage id
    * @return numeric value or false on failure
    */
   _getNumeric : function(id) {
   var tmp;
   return Conetserv.Options.storage ?
      ((tmp = parseInt(Conetserv.Options.storage[id])) > 0 ? tmp : false) :
      false;
   },

   /**
    * Private function for loading string data from localstorage
    * @param id local storage id
    * @param def value to be returned when no other is stored
    * @return string or def on failure
    */
   _getString : function(id, def) {
   return Conetserv.Options.storage ?
      (Conetserv.Options.storage[id] ? Conetserv.Options.storage[id] : def) :
      def;
   },

   /**
    * Private function for loading array data from localstorage
    * @param id local storage id
    * @param def value to be returned when no other is stored
    * @return array value or def on failure
    */
   _getArray : function(id, def) {
   return Conetserv.Options.storage ?
      (Conetserv.Options.storage[id] ? Conetserv.Options.storage[id].toString().split(";") : def) :
      def;
   },

   /**
    * Private function for loading boolean data from localstorage
    * @param id local storage id
    * @return boolean value or false on failure
    */
   _getBoolean : function(id) {
   return Conetserv.Options.storage ?
      this._toBool(Conetserv.Options.storage[id]) :
      false;
   },

   /**
    * Function for converting from string to boolean value
    * @param value string to be converted ( false | true )
    * @return boolean value
    */
   _toBool : function(value) {
      return value=="true" ? true : false;
   }
}

