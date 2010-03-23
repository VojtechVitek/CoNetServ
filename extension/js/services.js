var Services = {

   services: [{

         name: 'WIPmania.com - WorldIP API',

         method: 'GET',
         url: 'http://api.wipmania.com/',
         data: {},

         dataType: 'text',
         dataCharset: 'UTF-8',

         result: {
            externIpv4: null,
            countryCode: null
         },

         stable: '2010-03-23',

         parse: function(data) {

            var pattern = /^((?:\d{1,3}\.){3}\d{1,3})<br>([a-z]{1,3})$/gi;
            var result = pattern.exec(data);

            if (result && result[1])
               this.result.externIpv4 = result[1];

            if (result && result[2])
               this.result.countryCode = result[2];
         }

   }, {

         name: 'CGI script by Chris F.A. Johnson',

         method: 'GET',
         url: 'http://cfaj.freeshell.org/ipaddr.cgi',
         data: {},

         dataType: 'text',
         dataCharset: 'UTF-8',

         result: {
            externIpv4: null
         },

         stable: '2010-03-23',

         parse: function(data) {

            var pattern = /^((?:\d{1,3}\.){3}\d{1,3})/gi;
            var result = pattern.exec(data);

            if (result && result[1])
               this.result.externIpv4 = result[1];

         }
   }],

   running: 0,

   start: function(started_callback, result_callback, stopped_callback) {

      if (this.running == 0)
         this.running = this.services.length;
      else
         throw "Services already running..";

      started_callback();

      /* Run all services */
      for (var i = 0; i < this.services.length; ++i) {
         var This = this;
         var Service = this.services[i];
         $.ajax({
            Service: Service,
            type: Service.method,
            url: Service.url,
            processData: Service.data,
            dataType: Service.dataType,
            success: function(data) {
               // alert(data);
               // alert(this.Service.name);
               this.Service.parse(data);
               //$.extend(This.result, Service.result);
               result_callback(this.Service);
               if (--This.running == 0)
                  stopped_callback();
            },
            error: function() {
               if (--This.running == 0)
                  stopped_callback();
            }
         });
      }
   }

};
