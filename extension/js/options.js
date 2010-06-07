/**
 * Object for manipulating CoNetServ options
 */
var Options = {
   storage : null,
   toolbarButton : false,
   
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
      

      this.toolbarButton = this._toBool(this.storage['conetserv-settings-general-toolbox']);
      $("#settings-general-toolbox").attr("checked", this.toolbarButton);

   },

   /**
    * Saves options, depending on set values in forms.
    */
   save : function() {
      this.toolbarButton = $("#settings-general-toolbox").is(":checked");
      this.storage["conetserv-settings-general-toolbox"] = this.toolbarButton;
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

