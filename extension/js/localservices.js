/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/* LocalServices object */
Conetserv.LocalServices = {
   /* reading loop intervals */
   pingInterval : -1,
   ping6Interval : -1,
   tracerouteInterval : -1,
   traceroute6Interval : -1,
   whoisInterval : -1,
   nslookupInterval : -1,

   /* CoNetServ NPAPI plugin to interact native code with */
   plugin : 0,

   /* console text-boxes */
   pingConsole : 0,
   ping6Console : 0,
   tracerouteConsole : 0,
   traceroute6Console: 0,
   whoisConsole : 0,
   nslookupConsole : 0,

   /**
   * Read output of PING command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */
   readPing : function() {
      var received;
      try {
         received = this.plugin.readPing();
      }
      catch(e) {
         this.pingConsole.add(e);
         return;
      }
      if (received === false) {
         Conetserv.Ui.removeIcons(".local", ".ping");
         window.clearInterval(this.pingInterval);
         this.pingInterval = -1;
         return;
      }
      this.pingConsole.add(received);
      Plot.plotPing(received, 4);
   },

   /**
   * Read output of PING6 command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */
   readPing6 : function() {
      var received;
      try {
         received = this.plugin.readPing6();
      }
      catch(e) {
         this.ping6Console.add(e);
         return;
      }
      if (received === false) {
         Conetserv.Ui.removeIcons(".local", ".ping6");
         window.clearInterval(this.ping6Interval);
         this.ping6Interval = -1;
         return;
      }
      this.ping6Console.add(received);
      Plot.plotPing(received, 6);
   },

   /**
   * Read output of TRACEROUTE command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */
   readTraceroute : function()
   {
      var received;
      try {
         received = this.plugin.readTraceroute();
      }
      catch(e) {
         this.tracerouteConsole.add(e);
      }
      if (received === false) {
         Conetserv.Ui.removeIcons(".local", ".tracert");
         window.clearInterval(this.tracerouteInterval);
         this.tracerouteInterval = -1;
         return;
      }
      this.tracerouteConsole.add(received);
      Plot.plotTracert(received, 4);
   },

   /**
   * Read output of TRACEROUTE6 command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */
   readTraceroute6 : function() {
      var received;
      try {
         received = this.plugin.readTraceroute6();
      }
      catch(e) {
         this.traceroute6Console.add(e);
      }
      if (received === false) {
         Conetserv.Ui.removeIcons(".local", ".tracert6");
         window.clearInterval(this.traceroute6Interval);
         this.traceroute6Interval = -1;
         return;
      }
      this.traceroute6Console.add(received);
      Plot.plotTracert(received, 6);
   },

   /**
   * Read output of WHOIS command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */
   readWhois : function() {
      var received;
      try {
         received = this.plugin.readWhois();
      }
      catch(e) {
         this.whoisConsole.add(e);
      }
      if (received === false) {
         Conetserv.Ui.removeIcons(".local", ".whois");
         window.clearInterval(this.whoisInterval);
         this.whoisInterval = -1;
         return;
      }
      this.whoisConsole.add(received);
   },

   /**
   * Read output of NSLOOKUP command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */
   readNslookup : function() {
      var received;
      try {
         received = this.plugin.readNslookup();
      }
      catch(e) {
         this.nslookupConsole.add(e);
      }
      if (received === false) {
         Conetserv.Ui.removeIcons(".local", ".nslookup");
         window.clearInterval(this.nslookupInterval);
         this.nslookupInterval = -1;
         return;
      }
      this.nslookupConsole.add(received);
   },

   /**
   * Start all commands available at once.
   */
   startCommands : function() {
      //set value for commands
      if(!url.set(document.getElementById("local-url").value)) {
         document.getElementById("local-url").style.color="red";
         document.getElementById("local-url").focus();
         return;
      }
      this.startPing();
      this.startPing6();
      this.startTraceroute();
      this.startTraceroute6();
      this.startNslookup();
      this.startWhois();
   },

   /**
   * Start PING command
   */
   startPing : function() {
      if (this.pingInterval == -1) {
         try {
            this.pingConsole.clear();
            if (this.plugin.startPing(url.hostname)) {
               /* reset data and start animation */
               Plot.localPingData.reset();
               this.pingInterval = window.setInterval("Conetserv.LocalServices.readPing()", 500);
               this.readPing();

               Conetserv.Ui.addIcons(".local", ".ping", this.stopPing);
            } else {
               this.pingInterval = -1;
            }
         }
         catch(e) {
            this.pingConsole.add(e);
            this.pingInterval = -1;
         }
      }
   },

   /**
   * Start PING6 command
   */
   startPing6 : function() {
      if (this.ping6Interval == -1) {
         try {
            this.ping6Console.clear();
            if (this.plugin.startPing6(url.hostname)) {
               /* reset data and start animation */
               Plot.localPing6Data.reset();
               this.ping6Interval = window.setInterval("Conetserv.LocalServices.readPing6()", 500);
               this.readPing6();

               Conetserv.Ui.addIcons(".local", ".ping6", this.stopPing6);
            } else {
               this.ping6Interval = -1;
            }
         }
         catch(e) {
            this.ping6Console.add(e);
            this.ping6Interval = -1;
         }
      }
   },

   /**
   * Start TRACEROUTE command
   */
   startTraceroute : function() {
      if (this.tracerouteInterval == -1) {
         try {
            this.tracerouteConsole.clear();
            if (this.plugin.startTraceroute(url.hostname)) {
               Plot.localTraceData.reset();
               this.tracerouteInterval = window.setInterval("Conetserv.LocalServices.readTraceroute()", 500);
               this.readTraceroute();

               Conetserv.Ui.addIcons(".local", ".tracert", this.stopTraceroute);
            }
            else {
               this.tracerouteInterval = -1;
            }
         }
         catch(e) {
            this.tracerouteConsole.add(e);
            this.tracerouteInterval = -1;
         }
      }
   },

   /**
   * Start TRACEROUTE6 command
   */
   startTraceroute6 : function() {
      if (this.traceroute6Interval == -1) {
         try {
            this.traceroute6Console.clear();
            if (this.plugin.startTraceroute6(url.hostname)) {
               Plot.localTrace6Data.reset();
               this.traceroute6Interval = window.setInterval("Conetserv.LocalServices.readTraceroute6()", 500);
               this.readTraceroute6();

               Conetserv.Ui.addIcons(".local", ".tracert6", this.stopTraceroute6);
            }
            else {
               this.traceroute6Interval = -1;
            }
         }
         catch(e) {
            this.traceroute6Console.add(e);
            this.traceroute6Interval = -1;
         }
      }
   },

   /**
   * Start WHOIS command
   */
   startWhois : function() {
      if (this.whoisInterval == -1) {
         try {
            this.whoisConsole.clear();
            if (url.domain != '' && this.plugin.startWhois(url.domain)) {
               this.whoisInterval = window.setInterval("Conetserv.LocalServices.readWhois()", 500);
               this.readWhois();

               Conetserv.Ui.addIcons(".local", ".whois", stopWhois);
            }
            else {
               this.whoisInterval = -1;
            }
         }
         catch(e) {
            this.whoisConsole.add(e);
            this.whoisInterval = -1;
         }
      }
   },

   /**
   * Start NSLOOKUP command
   */
   startNslookup : function() {
      if (this.nslookupInterval == -1) {
         try {
            this.nslookupConsole.clear();
            if (this.plugin.startNslookup(url.hostname)) {
               this.nslookupInterval = window.setInterval("Conetserv.LocalServices.readNslookup()", 500);
               this.readNslookup();

               Conetserv.Ui.addIcons(".local", ".nslookup", this.stopNslookup);
            }
            else {
               this.nslookupInterval = -1;
            }
         }
         catch(e) {
            this.nslookupConsole.add(e);
            this.nslookupInterval = -1;
         }
      }
   },

   /**
   * Stop all commands available at once.
   */
   stopCommands : function() {
      this.stopPing();
      this.stopPing6();
      this.stopTraceroute();
      this.stopTraceroute6();
      this.stopNslookup();
      this.stopWhois();
   },

   /**
   * Stop PING command
   */
   stopPing : function() {
      if (this.pingInterval != -1) {
         try {
            this.plugin.stopPing();
         }
         catch(e) {
            this.pingConsole.add(e);
         }

         Conetserv.Ui.removeIcons(".local", ".ping");
         window.clearInterval(pingInterval);
         this.pingInterval = -1;
      }
   },

   /**
   * Stop PING6 command
   */
   stopPing6 : function() {
      if (this.ping6Interval != -1) {
         try {
            this.plugin.stopPing6();
         }
         catch(e) {
            this.ping6Console.add(e);
         }

         Conetserv.Ui.removeIcons(".local", ".ping6");
         window.clearInterval(ping6Interval);
         this.ping6Interval = -1;
      }
   },

   /**
   * Stop TRACEROUTE command
   */
   stopTraceroute : function() {
      if (this.tracerouteInterval != -1) {
         try {
            this.plugin.stopTraceroute();
         }
         catch(e) {
            this.tracerouteConsole.add(e);
         }

         Conetserv.Ui.removeIcons(".local", ".tracert");
         window.clearInterval(tracerouteInterval);
         this.tracerouteInterval = -1;
      }
   },

   /**
   * Stop TRACEROUTE6 command
   */
   stopTraceroute6 : function() {
      if (this.traceroute6Interval != -1) {
         try {
            this.plugin.stopTraceroute6();
         }
         catch(e) {
            this.traceroute6Console.add(e);
         }

         Conetserv.Ui.removeIcons(".local", ".tracert6");
         window.clearInterval(traceroute6Interval);
         this.traceroute6Interval = -1;
      }
   },

   /**
   * Stop WHOIS command
   */
   stopWhois : function() {
      if (this.whoisInterval != -1) {
         try {
            this.plugin.stopWhois();
         }
         catch(e) {
            this.whoisConsole.add(e);
         }

         Conetserv.Ui.removeIcons(".local", ".whois");
         window.clearInterval(whoisInterval);
         this.whoisInterval = -1;
      }
   },

   /**
   * Stop NSLOOKUP command
   */
   stopNslookup : function() {
      if (this.nslookupInterval != -1) {
         try {
            this.plugin.stopNslookup();
         }
         catch(e) {
            this.nslookupConsole.add(e);
         }

         Conetserv.Ui.removeIcons(".local", ".nslookup");
         window.clearInterval(nslookupInterval);
         this.nslookupInterval = -1;
      }
   }
}