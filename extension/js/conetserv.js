/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

Conetserv.plugin = false;

Conetserv.onReady = function() {

   /* Toggle tabs with opacity effect */
   //too slow and thus buggy; disable temporarily
   //$("#tabs").tabs({ fx: { opacity: 'toggle' } });

   $('#plugin-placeholder').append('<object id="conetserv" type="application/x-conetserv"></object>');

   this.plugin = document.getElementById("conetserv");

   /* init Conetserv.Url in Firefox */
   if ($.client.browser == "Firefox") {
      if (window && window.arguments && window.arguments[0] && Conetserv.Url.set(window.arguments[0])) {
         document.getElementById("local-url").value = Conetserv.Url.hostname;
      }
   /* init url in Chrome */
   } else if ($.client.browser == "Chrome") {
      if (chrome && chrome.tabs && chrome.tabs.getSelected) {
         chrome.tabs.getSelected(null, function(tab) {
            Conetserv.Url.set(tab.url);
            document.getElementById("local-url").value = Conetserv.Url.hostname;
         });
      }
   }
   /*
    * Initialize ui
    */
   this.Ui.checkAvailability();
   this.Ui.redraw();

   /*
    * Bind start button to start local services
    */
   $("#local-url-start").click(function() {
      Conetserv.LocalServices.stopCommands();
      Conetserv.LocalServices.startCommands();
   });

   /*
    * Start services on menuitem clicked
    */
    $('#external-info-header a').click(function(){
       Conetserv.ExternalInfo.start();
    });

   Conetserv.LocalServices.initialize();
   Conetserv.Plot.initialize();
   this.Options.initialize();
};

Conetserv.onLoad = function() {
      /*
    * Check autostart - on true start services
    */
   if(this.Options.autostart) {
      Conetserv.LocalServices.startCommands();
   }

   /*
    * If main page is eternal info, start them
    */
   if(this.Options.frontPageParent == "external-info") {
       setTimeout("startExternalInfo()",250);
   }
}