/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

Conetserv.plugin = false;
Conetserv.version = "2.0.0";
Conetserv.changelog = "* Object-oriented API to JS (availability for 3rd party) <br/>\
* Independent objects to run programs and commands with options <br/>\
* User-friendly error messages <br/>\
* Installation guides for missing programs (OS, platform, distro)"

Conetserv.onReady = function() {

   /* Toggle tabs with opacity effect */
   //too slow and thus buggy; disable temporarily
   //$("#tabs").tabs({ fx: { opacity: 'toggle' } });

   $('#plugin-placeholder').append('<object id="conetserv" type="application/x-conetserv"></object>');

   /* Inicialize conetserv plugin */
   this.plugin = document.getElementById("conetserv");

   /* init Conetserv.Url in Firefox */
   if ($.client.browser == "Firefox") {
      if (window && window.arguments && window.arguments[0] && Conetserv.Url.set(window.arguments[0])) {
         if(Conetserv.LocalServices.enabled) {
            document.getElementById("local-url").value = Conetserv.Url.hostname;
         }
         if(Conetserv.ExternalServices.enabled) {
            document.getElementById("external-url").value = Conetserv.Url.hostname;
         }
      }
   /* init url in Chrome */
   } else if ($.client.browser == "Chrome") {
      if (chrome && chrome.tabs && chrome.tabs.getSelected) {
         chrome.tabs.getSelected(null, function(tab) {
            Conetserv.Url.set(tab.url);
            if(Conetserv.LocalServices.enabled) {
               document.getElementById("local-url").value = Conetserv.Url.hostname;
            }
            if(Conetserv.ExternalServices.enabled) {

               document.getElementById("external-url").value = Conetserv.Url.hostname;
            }
         });
      }
   }
   /*
    * Initialize ui
    */
   this.Ui.checkAvailability();
   this.Ui.redraw();

   Conetserv.LocalServices.initialize();
   Conetserv.ExternalServices.initialize();
   Conetserv.LocalInfo.initialize();

   Conetserv.Options.initialize();

   Conetserv.Plot.initialize();
};

Conetserv.onLoad = function() {
   /*
    * Check autostart - on true start services
    */
   if(this.Options.autostart()) {
      /*
       * Start services on main page
       */
      if(this.Options.frontPageParent() == "local-services") {
         setTimeout("Conetserv.LocalServices.startCommands()", 250);
      }
      else if(this.Options.frontPageParent() == "external-services") {
         setTimeout("Conetserv.ExternalServices.start()", 250);
      }
   }
   if(this.Options.frontPageParent() == "external-info") {
      setTimeout("Conetserv.ExternalInfo.start()", 250);
   }
   if(this.Options.frontPageParent() == "local-info") {
      setTimeout("Conetserv.LocalInfo.start()", 250)
   }
}