/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/**
 * Object for manipulating CoNetServ options
 */
Conetserv.Options = {
   storage : false,
   toolbarButton : false,
   autostart : false,
   frontPageParent : false,
   frontPageChild : false,
   skin: false,
   ext_services: false,
   ext_services_router: false,
   version: false,

   //enums for easier argument passing to functions
   enums : {
      TOOLBAR_BUTTON :        0,
      AUTOSTART :             1,
      FRONTPAGE_PARENT :      2,
      FRONTPAGE_CHILD :       3,
      SKIN :                  4,
      EXT_SERVICES :          5,
      EXT_SERVICES_ROUTER :   6
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
         this.skin = $("#settings-general-skin input").val();
         ext_services: "true;true;;true;;;;;;;true;true;;;;;;;;;true;;;true;;;;;;;true;true";
         ext_services_router: "r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp;;;;;;;;;r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp";

         Conetserv.Ui.divError("#settings", "<strong>Local storage initialization has failed.\
            </strong> <br /><br />\
            Please make sure you are using supported web browser.");
         
         return;
      }

      /* Conetserv is run for the first time */
      if(!this.storage['conetserv-version'] || this.storage['conetserv-version'].toString() == '') {
         setTimeout('Conetserv.Ui.showDialog("<strong>Welcome to CoNetServ extension.</strong><br />Settings has been set to default values.")', 500);
      }
      /* Conetserv has been updated */
      else if(this.storage['conetserv-version'] != Conetserv.version){
         setTimeout('Conetserv.Ui.showDialog("CoNetServ has been updated to version " + Conetserv.version, "Release notes: <br/>" + Conetserv.changelog)', 500);
      }
      
      /* Check, if version stored in local storage is compatible with current version */
      if(!this.storage['conetserv-version'] || this.storage['conetserv-version'].toString().substr(0, 1) != Conetserv.version.substr(0,1)) {
         this.storage['conetserv-version'] = Conetserv.version;
         this.storage["conetserv-settings-external-services_router"] = "";

         this.storage['conetserv-settings-general-toolbox'] = false;
         this.storage['conetserv-settings-general-autostart'] = false;

         this.storage['conetserv-settings-general-frontpage'] = "local-services";
         this.storage['conetserv-settings-general-frontpage-child'] = "local-ping-input";

         this.storage['conetserv-settings-general-skin'] = $("#settings-general-skin input").val();

         this.storage["conetserv-settings-external-services"] = "true;true;;true;;;;;;;true;true;;;;;;;;;true;;;true;;;;;;;true;true";
         this.storage["conetserv-settings-external-services_router"] = "r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp;;;;;;;;;r01ext;bgp-isp;GP0;Amsterdam;;;;;;;r01ext;bgp-isp";
         
      }
      
      this.toolbarButton = this._toBool(this.storage['conetserv-settings-general-toolbox']);
      $("#settings-general-toolbox").attr("checked", this.toolbarButton);

      this.autostart = this._toBool(this.storage['conetserv-settings-general-autostart']);
      $("#settings-general-autostart").attr("checked", this.autostart);

      /* redraw options page */
      this.frontPageParent = this.storage['conetserv-settings-general-frontpage'];

      this.frontPageChild = this.storage['conetserv-settings-general-frontpage-child'];
      $("#frontpage-" + this.frontPageParent).attr("checked", true);
      Conetserv.Ui.redrawOptions();
      $("#frontpage-" + this.frontPageChild).attr("checked", true);

      /* depending on options page, set the active tab */
      $("#tabs").tabs('select', "#" + this.frontPageParent);
      $("#" + this.frontPageParent).ready(function() {
         $("#" + Conetserv.Options.frontPageChild).attr("checked", true);
      });

      /* skin options */
      this.skin = this.storage['conetserv-settings-general-skin'] ? this.storage['conetserv-settings-general-skin'] : $("#settings-general-skin input").val();
      $("#settings-general-skin input[value="+this.skin+"]").attr("checked", true);

      /* external services */
      this.ext_services = this.storage["conetserv-settings-external-services"] ? 
         this.storage["conetserv-settings-external-services"].toString().split(";") : [];

      /* set buttons checked on external services page */
      for(var key in this.ext_services) {
         if(this.ext_services[key]) {
            $("#settings-external-services-form input[value="+key+"]").attr("checked", true);
         }
      }
      /* external services router */
      this.ext_services_router = this.storage["conetserv-settings-external-services_router"] ? 
         this.storage["conetserv-settings-external-services_router"].toString().split(";") : [];

      for(key in this.ext_services_router) {
         if(this.ext_services_router[key]) {
            switch(parseInt(key)) {
               case Conetserv.LookingGlass.enums.CERN_PING_V4:
                  $("#settings-external-services-cern-ipv4-router").attr("value", this.ext_services_router[key]);
                  break;
               case Conetserv.LookingGlass.enums.SPARKLE_PING_V4:
                  $("#settings-external-services-sparkle-ipv4-router").attr("value", this.ext_services_router[key]);
                  break;
               case Conetserv.LookingGlass.enums.ATMAN_PING_V4:
                  $("#settings-external-services-atman-ipv4-router").attr("value", this.ext_services_router[key]);
                  break;
               case Conetserv.LookingGlass.enums.ILAN_PING_V4:
                  $("#settings-external-services-ilan-ipv4-router").attr("value", this.ext_services_router[key]);
                  break;
              case Conetserv.LookingGlass.enums.CERN_PING_V6:
                  $("#settings-external-services-cern-ipv6-router").attr("value", this.ext_services_router[key]);
                  break;
              case Conetserv.LookingGlass.enums.ATMAN_PING_V4:
                  $("#settings-external-services-atman-ipv6-router").attr("value", this.ext_services_router[key]);
                  break;
            }
         }
      }

      this.version = this.storage["conetserv-version"];
   },

   /**
    * Saves options, depending on set values in forms.
    */
   save : function(option) {
      if(!this.storage)
         return false;
      switch(option) {
         case this.enums.TOOLBAR_BUTTON:
            this.toolbarButton = $("#settings-general-toolbox").is(":checked");
            this.storage["conetserv-settings-general-toolbox"] = this.toolbarButton;
            break;
         case this.enums.AUTOSTART:
            this.autostart = $("#settings-general-autostart").is(":checked");
            this.storage["conetserv-settings-general-autostart"] = this.autostart;
            break;
         case this.enums.FRONTPAGE_PARENT:
            this.frontPageParent = $("#settings-general-frontpage input:checked").val();
            this.storage["conetserv-settings-general-frontpage"] = this.frontPageParent;
            break;
         case this.enums.FRONTPAGE_CHILD:
            this.frontPageChild = $("#settings-general-frontpage-children input:checked").val();
            this.storage["conetserv-settings-general-frontpage-child"] = this.frontPageChild;
            break;
         case this.enums.SKIN:
            this.skin = $("#settings-general-skin input:checked").val();
            this.storage["conetserv-settings-general-skin"] = this.skin;
            Conetserv.Ui.reloadSkin();
            break;
         case this.enums.EXT_SERVICES:
            /* clean ext_services and fill them depending on checked services */
            this.ext_services = [];
            
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
               Conetserv.Options.ext_services[$(this).attr("value")] = true;
            });

            /* if maximum alowed services has already been reached, return false */
            if(ping > 3 || ping6 > 3 || tracert > 2 || tracert6 > 2) {
               Conetserv.Ui.showDialog("Warning!", "You can allow only 3 ping services and 2 traceroute services in both IPv4 and IPv6.");
               return false;
            }

            /* join array and save to storage */
            this.storage["conetserv-settings-external-services"] = this.ext_services.join(";");
            break;
         case this.enums.EXT_SERVICES_ROUTER:
            $("#settings-external-services-form select").each(function(){
               switch($(this).attr("id")) {
                  case "settings-external-services-cern-ipv4-router":
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.CERN_PING_V4] = $(this).attr("value");
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.CERN_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-sparkle-ipv4-router":
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.SPARKLE_PING_V4] = $(this).attr("value");
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.SPARKLE_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-atman-ipv4-router":
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.ATMAN_PING_V4] = $(this).attr("value");
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.ATMAN_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-ilan-ipv4-router":
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.ILAN_PING_V4] = $(this).attr("value");
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.ILAN_TRACERT_V4] = $(this).attr("value");
                     break;
                  case "settings-external-services-cern-ipv6-router":
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.CERN_PING_V6] = $(this).attr("value");
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.CERN_TRACERT_V6] = $(this).attr("value");
                     break;
                  case "settings-external-services-atman-ipv6-router":
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.ATMAN_PING_V6] = $(this).attr("value");
                     Conetserv.Options.ext_services_router[Conetserv.LookingGlass.enums.ATMAN_TRACERT_V6] = $(this).attr("value");
                     break;
               }
            });
            this.storage["conetserv-settings-external-services_router"] = this.ext_services_router.join(";");
            break;

      }

      return true;
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

