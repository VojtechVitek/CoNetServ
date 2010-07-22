/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

Conetserv.onReady = function() {

   /* Toggle tabs with opacity effect */
   //too slow and thus buggy; disable temporarily
   //$("#tabs").tabs({ fx: { opacity: 'toggle' } });

   $('#plugin-placeholder').append('<object id="conetserv" type="application/x-conetserv"></object>');

   /* CoNetServ object - NPAPI plugin */
   this.LocalServices.plugin = document.getElementById("conetserv");

   /* console text-boxes */
   this.LocalServices.pingConsole = new console("local-ping-console");
   this.LocalServices.pingConsole.maxRows = 15;
   this.LocalServices.ping6Console = new console("local-ping6-console");
   this.LocalServices.ping6Console.maxRows = 15;
   this.LocalServices.tracerouteConsole = new console("local-tracert-console");
   this.LocalServices.traceroute6Console = new console("local-tracert6-console");
   this.LocalServices.whoisConsole = new console("local-whois-console");
   this.LocalServices.nslookupConsole = new console("local-nslookup-console");

   /* init url in Firefox */
   if ($.client.browser == "Firefox") {
      if (window && window.arguments && window.arguments[0] && url.set(window.arguments[0])) {
         document.getElementById("local-url").value = url.hostname;
      }
   /* init url in Chrome */
   } else if ($.client.browser == "Chrome") {
      if (chrome && chrome.tabs && chrome.tabs.getSelected) {
         chrome.tabs.getSelected(null, function(tab) {
            url.set(tab.url);
            document.getElementById("local-url").value = url.hostname;
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

   Plot.initialize();
   this.Options.initialize();
};

Conetserv.onLoad = function() {
      /*
    * Check autostart - on true start services
    */
   if(this.Options.autostart) {
      startCommands();
   }

   /*
    * If main page is eternal info, start them
    */
   if(this.Options.frontPageParent == "external-info") {
       setTimeout("startExternalInfo()",250);
   }
}