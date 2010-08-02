/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/* LocalServices object */
Conetserv.LocalServices = {
   /* CoNetServ NPAPI plugin to interact native code with */
   plugin : 0,

   /**
   * Start all commands available at once.
   */
   startCommands : function() {
      //set value for commands
      if(!Conetserv.Url.set(document.getElementById("local-url").value)) {
         document.getElementById("local-url").style.color="red";
         document.getElementById("local-url").focus();
         return;
      }

      this.start(this.Ping);
      this.start(this.Ping6);
      this.start(this.Traceroute);
      this.start(this.Traceroute6);
      this.start(this.Nslookup);
      this.start(this.Whois);
   },

   stopCommands : function() {
      this.stop(this.Ping);
      this.stop(this.Ping6);
      this.stop(this.Traceroute);
      this.stop(this.Traceroute6);
      this.stop(this.Nslookup);
      this.stop(this.Whois);
   },


   //@TODO Domain x hostname
   start : function (service) {
      if (service.interval == -1) {
         service.before_begin();
         try {
            service.console.clear();
            // Create function from service details and evaluate it
            if (eval("Conetserv.LocalServices.plugin." + service.name + ".start(" + service.argument + ")")) {
               service.interval = window.setInterval("Conetserv.LocalServices.read(" + service.object + ")", 500);
               this.read(service);

               Conetserv.Ui.addIcons(".local", service.cls, "Conetserv.LocalServices.stop(" + service.object + ")");
            }
            else {
               service.interval = -1;
            }
         }
         catch(e) {
            service.console.add(e);
            service.interval = -1;
         }
      }
   },

   /**
   * Read output of PING command
   * \return String data if successful (could be zero length),
   *         false otherwise (false indicates not yet started or stopped process)
   */

   read : function(service) {
      var received;
      try {
         // Create function from service details and evaluate it
         received = eval("Conetserv.LocalServices.plugin." + service.name + ".read()");
      }
      catch(e) {
         service.console.add(e);
      }

      //@TODO fix after v-teq commits new core!!
      if (0) {
         Conetserv.Ui.removeIcons(".local", service.cls);
         window.clearInterval(service.interval);
         service.interval = -1;
         return;
      }
      service.console.add(received);

      service.after_read(received);

   },

   stop : function (service) {
      if (service.interval != -1) {
         try {
            eval("Conetserv.LocalServices.plugin." + service.name + ".stop()")
         }
         catch(e) {
            service.console.add(e);
         }

         Conetserv.Ui.removeIcons(".local", service.cls);
         window.clearInterval(service.interval);
         service.interval = -1;
      }
   },

   initialize : function () {
      /* CoNetServ object - NPAPI plugin */
      this.plugin = document.getElementById("conetserv");

      /* console text-boxes */
      this.Ping.console = new Conetserv.Console("local-ping-console");
      this.Ping.console.maxRows = 15;
      this.Ping6.console = new Conetserv.Console("local-ping6-console");
      this.Ping6.console.maxRows = 15;
      this.Traceroute.console = new Conetserv.Console("local-tracert-console");
      this.Traceroute6.console = new Conetserv.Console("local-tracert6-console");
      this.Nslookup.console = new Conetserv.Console("local-nslookup-console");
      this.Whois.console = new Conetserv.Console("local-whois-console");
   }
}

Conetserv.LocalServices.Ping = {
   interval : -1,    //reading loop interval
   console : 0,     //text console
   object : 'Conetserv.LocalServices.Ping',                //object full name -- !! IMPORTANT
   cls : '.ping',                                                              //class for icons
   name: 'ping',                                                            //name for calling npapi functions
   argument:'Conetserv.Url.hostname',                        //parameter to be passed to npapi functions

   before_begin : function() {                                  //extra commands to be executed before service is started
      Conetserv.Plot.localPingData.reset();
   },

   after_read : function(received) {                        //extra commands to be done in the end of read function
      Conetserv.Plot.plotPing(received, 4);
   }
}

Conetserv.LocalServices.Ping6 = {
   interval : -1,    //reading loop interval
   console : 0,     //text console
   object : 'Conetserv.LocalServices.Ping6',                //object full name -- !! IMPORTANT
   cls : '.ping6',                                                              //class for icons
   name: 'ping6',                                                            //name for calling npapi functions
   argument:'Conetserv.Url.hostname',                        //parameter to be passed to npapi functions

   before_begin : function() {                                  //extra commands to be executed before service is started
      Conetserv.Plot.localPing6Data.reset();
   },

   after_read : function(received) {
      Conetserv.Plot.plotPing(received, 6);
   }
}

Conetserv.LocalServices.Traceroute = {
   interval : -1,    //reading loop interval
   console : 0,     //text console
   object : 'Conetserv.LocalServices.Traceroute',                //object full name -- !! IMPORTANT
   cls : '.tracert',                                                              //class for icons
   name: 'traceroute',                                                            //name for calling npapi functions
   argument:'Conetserv.Url.hostname',                        //parameter to be passed to npapi functions

   before_begin : function() {                                  //extra commands to be executed before service is started
      Conetserv.Plot.localTraceData.reset();
   },

   after_read : function(received) {
      Conetserv.Plot.plotTracert(received, 4);
   }
}

Conetserv.LocalServices.Traceroute6 = {
   interval : -1,    //reading loop interval
   console : 0,     //text console
   object : 'Conetserv.LocalServices.Traceroute6',                //object full name -- !! IMPORTANT
   cls : '.tracert6',                                                              //class for icons
   name: 'traceroute6',                                                            //name for calling npapi functions
   argument:'Conetserv.Url.hostname',                        //parameter to be passed to npapi functions

   before_begin : function() {                                  //extra commands to be executed before service is started
      Conetserv.Plot.localTrace6Data.reset();
   },

   after_read : function(received) {
      Conetserv.Plot.plotTracert(received, 6);
   }
}

Conetserv.LocalServices.Nslookup = {
   interval : -1,    //reading loop interval
   console : 0,     //text console
   object : 'Conetserv.LocalServices.Nslookup',                //object full name -- !! IMPORTANT
   cls : '.nslookup',                                                              //class for icons
   name: 'nslookup',                                                            //name for calling npapi functions
   argument:'Conetserv.Url.hostname',                        //parameter to be passed to npapi functions

   before_begin : function() {                                  //extra commands to be executed before service is started
   },

   after_read : function(received) {
   }
}

Conetserv.LocalServices.Whois = {
   interval : -1,    //reading loop interval
   console : 0,     //text console
   object : 'Conetserv.LocalServices.Whois',                //object full name -- !! IMPORTANT
   cls : '.whois',                                                              //class for icons
   name: 'nslookup',                                                            //name for calling npapi functions
   argument:'Conetserv.Url.domain',                        //parameter to be passed to npapi functions

   before_begin : function() {                                  //extra commands to be executed before service is started
   },

   after_read : function(received) {
      Conetserv.Plot.plotTracert(received, 4);
   }
}

Conetserv.LocalServices.Nmap = {

   }