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

   start : function (service) {
      if (service.interval == -1) {
         service.before_begin();
         try {
            service.console.clear();
            // Create function from service details and evaluate it
            if (eval("service.process = Conetserv.LocalServices.plugin." + service.identifier + ".start(" + service.argument + ")")) {
               service.interval = window.setInterval(this.read, 500, service);
               Conetserv.Ui.addIcons(".local", service.cls, "Conetserv.LocalServices.stop(" + service + ")");
               this.read(service);
            } else {
               service.interval = -1;
            }
         } catch(e) {
            service.console.add(e);
            service.interval = -1;
         }
      }
   },

   read : function(service) {
      var received;
      try {
         // Create function from service details and evaluate it
         received = service.process.read();
      } catch(e) {
         service.console.add(e);
      }

      if (typeof(received) == 'string') {
         service.console.add(received);
         service.after_read(received);
      } else {
         if (!service.process.running) {
            this.stop(service);
         }
      }
   },

   stop : function (service) {
      if (service.interval != -1) {
         try {
            service.process.stop();
         }
         catch(e) {
            service.console.add(e);
         }
         Conetserv.Ui.removeIcons(".local", service.cls);
         delete service.process;
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
   interval : -1,                            //reading loop interval
   console : 0,                              //text console
   process : false,                          //running process to read data from
   cls : '.ping',                            //class for icons
   identifier: 'ping',                       //name for calling npapi functions
   argument:'Conetserv.Url.hostname',        //parameter to be passed to npapi functions

   before_begin : function() {               //extra commands to be executed before service is started
      Conetserv.Plot.localPingData.reset();
   },

   after_read : function(received) {         //extra commands to be done in the end of read function
      Conetserv.Plot.plotPing(received, 4);
   }
}

Conetserv.LocalServices.Ping6 = {
   interval : -1,                            //reading loop interval
   console : 0,                              //text console
   process : false,                          //running process to read data from
   cls : '.ping6',                           //class for icons
   identifier: 'ping6',                            //name for calling npapi functions
   argument:'Conetserv.Url.hostname',        //parameter to be passed to npapi functions

   before_begin : function() {               //extra commands to be executed before service is started
      Conetserv.Plot.localPing6Data.reset();
   },

   after_read : function(received) {
      Conetserv.Plot.plotPing(received, 6);
   }
}

Conetserv.LocalServices.Traceroute = {
   interval : -1,                            //reading loop interval
   console : 0,                              //text console
   process : false,                          //running process to read data from
   cls : '.tracert',                         //class for icons
   identifier: 'traceroute',                 //name for calling npapi functions
   argument:'Conetserv.Url.hostname',        //parameter to be passed to npapi functions

   before_begin : function() {               //extra commands to be executed before service is started
      Conetserv.Plot.localTraceData.reset();
   },

   after_read : function(received) {
      Conetserv.Plot.plotTracert(received, 4);
   }
}

Conetserv.LocalServices.Traceroute6 = {
   interval : -1,                            //reading loop interval
   console : 0,                              //text console
   process : false,                          //running process to read data from
   cls : '.tracert6',                        //class for icons
   identifier: 'traceroute6',                //name for calling npapi functions
   argument:'Conetserv.Url.hostname',        //parameter to be passed to npapi functions

   before_begin : function() {               //extra commands to be executed before service is started
      Conetserv.Plot.localTrace6Data.reset();
   },

   after_read : function(received) {
      Conetserv.Plot.plotTracert(received, 6);
   }
}

Conetserv.LocalServices.Nslookup = {
   interval : -1,                           //reading loop interval
   console : 0,                             //text console
   process : false,                          //running process to read data from
   cls : '.nslookup',                       //class for icons
   identifier: 'nslookup',                  //name for calling npapi functions
   argument:'Conetserv.Url.hostname',       //parameter to be passed to npapi functions

   before_begin : function() {              //extra commands to be executed before service is started
   },

   after_read : function(received) {
   }
}

Conetserv.LocalServices.Whois = {
   interval : -1,                           //reading loop interval
   console : 0,                             //text console
   process : false,                          //running process to read data from
   cls : '.whois',                          //class for icons
   identifier: 'nslookup',                  //name for calling npapi functions
   argument:'Conetserv.Url.domain',         //parameter to be passed to npapi functions

   before_begin : function() {              //extra commands to be executed before service is started
   },

   after_read : function(received) {
      Conetserv.Plot.plotTracert(received, 4);
   }
}

Conetserv.LocalServices.Nmap = {

}