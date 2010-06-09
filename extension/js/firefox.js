/**
 * Object containing functions for Firefox browser
 */
var Firefox = {
   appendButtonInToolbar:function(buttonId, toolbarId) {
      var toolbar = document.getElementById(toolbarId);
      var button = document.getElementById(buttonId);
      var before = document.getElementById("urlbar-container");
      if(button) {
         var parentBar = button.parentNode;
         if(parentBar && parentBar != toolbar) {
            var newset = this.removeButtonFromToolbarCurrentSet(parentBar,buttonId);
         }
         toolbar.insertBefore(button, before);
      }else{
         toolbar.insertItem(buttonId);
      }

      this.appendButtonInToolbarCurrentSet(toolbar,buttonId);
   },

   appendButtonInToolbarCurrentSet:function(toolbar, buttonId) {
      var oldset = toolbar.getAttribute("currentset");
      var newset = "";
      if(oldset && oldset!="") {
         newset = oldset + ",";
      }
      newset += buttonId;
      toolbar.setAttribute("currentset", newset);
      document.persist(toolbar.id,"currentset");
      return newset;
   },


   removeButtonFromToolbarCurrentSet:function(toolbar, buttonId) {
      var oldset = toolbar.getAttribute("currentset");
      if(!oldset || oldset=="" || oldset.indexOf(buttonId) == -1) return oldset;
      var reg = new RegExp(buttonId+",?", "gi");
      var newset = oldset.replace(reg,"");
      if (newset.charAt(newset.length-1) == ",") {
         newset = newset.substring(0, newset.length - 1);
      }

      toolbar.setAttribute("currentset", newset);
      document.persist(toolbar.id,"currentset");
      return newset;
   }
}

