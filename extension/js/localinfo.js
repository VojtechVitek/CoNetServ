/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/* External info object */
Conetserv.LocalInfo = {
   enabled : true,
   running : 0,

   initialize : function() {
      if(!this.enabled)
         return;

      this.Ipconfig.console = new Conetserv.Console("local-ipconfig-console");
      this.Nmap.console = new Conetserv.Console("local-nmap-console");
   },

   start : function() {
      if(!this.enabled || this.running)
         return false;

      this.startService(this.Ipconfig);
      //this.startService(this.Nmap);

      return true;
   },

   stop : function() {
      if(!this.enabled || this.running)
         return false;

      this.stopService(this.Ipconfig);
      //this.stopService(this.Nmap);

      return true
   },

   startService : function(service) {
      if (service.enabled && service.interval == -1) {
         Conetserv.LocalInfo.running++;

         
         service.before_begin();
         service.finished = false;

         service.console.clear();

         service.console.inputTimerErr(service.object + ".console", 3, "<strong>Service has not returned any output yet.</strong> ");

         try {
            // Create function from service details and evaluate it
            if (eval("service.process = Conetserv.plugin." + service.identifier + ".start()")) {
               service.interval = window.setInterval(this.readService, 500, service);
               Conetserv.Ui.addIcons(".local", service.cls, '', '', 'ui-icon-clock');
               this.readService(service);
            } else {
               service.interval = -1;
            }
         } catch(e) {
            service.console.setErr(e);
            service.interval = -1;
         }

         return true;
      }
      else {
         return false;
      }
   },

   readService : function(service) {
      if (!service.enabled) {
         return false;
      }

      var received;
      try {
         received = service.process.read();
      } catch(e) {
         service.console.setErr(e);
      }

      if (typeof(received) == 'string') {
         service.console.add(received);
         service.after_read(received);
      } else {
         if (!service.process.running) {
            if(service.console.rowCount <= 2) {
               var err = service.console.code;
               service.console.clear();
               service.console.setErr("<strong>Service has most probably encountered an error with following output: </strong> <br/></br>"
                  + err);
            }
            service.finished = true;
            Conetserv.LocalInfo.stopService(service);
         }
      }

      return true;
   },

   stopService : function(service) {
      if (service.enabled && service.interval != -1) {

         try {
            delete service.process;
         }
         catch(e) {
            service.console.setErr(e);
         }

         Conetserv.Ui.removeIcons(".local", service.cls);

         service.console.touched = true;
         /* if service has been stopped manually, set error to console */
         if(!service.finished) {
            service.console.setErr("<strong>Service has been terminated before its finish.</strong>");
         }

         window.clearInterval(service.interval);
         service.interval = -1;
         if(--Conetserv.LocalServices.running == 0)
         {
         }

         return true;
      }
      return false;
   }
}

Conetserv.LocalInfo.Ipconfig = {
   enabled: true,                              //defines, if service will be started
   interval: -1,                            //reading loop interval
   finished: false,                         //saying, if service has finished its run
   console: false,                          //text console
   process: false,                          //running process to read data from
   cls: '.ipconfig',                         //class for icons
   object: 'Conetserv.LocalInfo.Ipconfig', //object name
   identifier: 'ipconfig',                     //name for calling npapi functions
   argument:'',       //parameter to be passed to npapi functions

   before_begin: function() {               //extra commands to be executed before service is started

   },

   after_read: function(received) {

   }
}

Conetserv.LocalInfo.Nmap = {
   enabled: true,                              //defines, if service will be started
   interval: -1,                            //reading loop interval
   finished: false,                         //saying, if service has finished its run
   console: false,                          //text console
   process: false,                          //running process to read data from
   cls: '.nmap',                         //class for icons
   object: 'Conetserv.LocalInfo.Nmap', //object name
   identifier: 'nmap',                     //name for calling npapi functions
   argument:'',       //parameter to be passed to npapi functions

   before_begin: function() {               //extra commands to be executed before service is started
      Conetserv.plugin.nslookup.query = 1;
   },

   after_read: function(received) {
      
   }
}

