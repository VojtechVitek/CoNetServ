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
         return;
      }

      this.toolbarButton = this._toBool(this.storage['conetserv-settings-general-toolbox']);
      $("#settings-general-toolbox").attr("checked", this.toolbarButton);

      this.autostart = this._toBool(this.storage['conetserv-settings-general-autostart']);
      $("#settings-general-autostart").attr("checked", this.autostart);

      /* redraw options page */
      this.frontPageParent = this.storage['conetserv-settings-general-frontpage'];
      if(!this.frontPageParent) {
         this.frontPageParent = "local-services";
      }
      this.frontPageChild = this.storage['conetserv-settings-general-frontpage-child'];
      $("#frontpage-" + this.frontPageParent).attr("checked", true);
      Ui.redrawOptions();
      $("#frontpage-" + this.frontPageChild).attr("checked", true);

      /* depending on options page, set the active tab */
      $("#tabs").tabs('select', "#" + this.frontPageParent);
      $("#" + this.frontPageParent).ready(function() {
         $("#" + Conetserv.Options.frontPageChild).attr("checked", true);
      });

      /* skin options */
      this.skin = this.storage['conetserv-settings-general-skin'];
      if(!this.skin) {
         this.skin = $("#settings-general-skin input").val();
      }
      $("#settings-general-skin input[value="+this.skin+"]").attr("checked", true);
   },

   /**
    * Saves options, depending on set values in forms.
    */
   save : function() {
      if(!this.storage)
         return;

      this.toolbarButton = $("#settings-general-toolbox").is(":checked");
      this.storage["conetserv-settings-general-toolbox"] = this.toolbarButton;

      this.autostart = $("#settings-general-autostart").is(":checked");
      this.storage["conetserv-settings-general-autostart"] = this.autostart;

      /* frontpage */
      this.frontPageParent = $("#settings-general-frontpage input:checked").val();
      this.frontPageChild = $("#settings-general-frontpage-children input:checked").val();
      this.storage["conetserv-settings-general-frontpage"] = this.frontPageParent;
      this.storage["conetserv-settings-general-frontpage-child"] = this.frontPageChild;

      /* skin */
      this.skin = $("#settings-general-skin input:checked").val();
      this.storage["conetserv-settings-general-skin"] = this.skin;
      Ui.reloadSkin();

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

