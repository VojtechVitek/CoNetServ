/**
 * Object for manipulating CoNetServ options
 */
var Options = {
   storage : false,
   toolbarButton : false,
   frontPageParent : false,
   frontPageChild : false,
   
   /**
    * Inicializes CoNetServ options
    */
   inicialize : function() {
      // fix localStorage for firefox
      if($.client.browser == "Firefox") {
         this.storage = globalStorage[location.hostname];
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

      /* redraw options page */
      this.frontPageParent = this.storage['conetserv-settings-general-frontpage'];
      this.frontPageChild = this.storage['conetserv-settings-general-frontpage-child'];
      $("#frontpage-" + this.frontPageParent).attr("checked", true);
      Ui.redrawOptions();
      $("#frontpage-" + this.frontPageChild).attr("checked", true);

      /* depending on options page, set the active tab */
      //$("#" + this.frontPageParent+"-header").addClass("ui-tabs-selected ui-state-active");//addClass("ui-tabs-hide");
      //$("#" + this.frontPageParent).removeClass("ui-tabs-hide");//addClass("ui-tabs-hide");
      //$("#tabs div.ui-tabs-panel:not(.ui-tabs-hide)").attr("id");
      //$("#tabs").tabs('select', 5);
   },

   /**
    * Saves options, depending on set values in forms.
    */
   save : function() {
      if(!this.storage)
         return;

      this.toolbarButton = $("#settings-general-toolbox").is(":checked");
      this.storage["conetserv-settings-general-toolbox"] = this.toolbarButton;

      /* frontpage */
      this.frontPageParent = $("#settings-general-frontpage input:checked").val();
      this.frontPageChild = $("#settings-general-frontpage-children input:checked").val();
      this.storage["conetserv-settings-general-frontpage"] = this.frontPageParent;
      this.storage["conetserv-settings-general-frontpage-child"] = this.frontPageChild;

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

