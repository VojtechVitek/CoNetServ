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
   },

   /**
    * Saves options, depending on set values in forms.
    */
   save : function() {
      if(!this.storage)
         return;

      this.toolbarButton = $("#settings-general-toolbox").is(":checked");
      this.storage["conetserv-settings-general-toolbox"] = this.toolbarButton;

      var tabpanel = $("#settings-general-frontpage input:checked").val();
      alert(tabpanel);
      alert($("#" . tabpanel));

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

