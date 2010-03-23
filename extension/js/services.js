var Services = {

   services: [{

         name: 'CGI script by Chris F.A. Johnson',

         method: 'GET',
         url: 'http://cfaj.freeshell.org/ipaddr.cgi',
         data: {},

         dataType: 'text',
         dataCharset: 'UTF-8',

         result: {
            externIpv4: null
         },

         stable: '2010-03-18',

         parse: function(data) {
            this.result.externIpv4 = data.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/gi)[0];
         }
   }],

   running: 0,

   start: function(start_callback, update_callback, stop_callback) {

      start_callback();

      /* Run all services */
      this.running += this.services.length;
      for (var i = 0; i < this.services.length; ++i) {
         var This = this;
         var Service = this.services[i];
         $.ajax({
            type: Service.method,
            url: Service.url,
            processData: Service.data,
            dataType: Service.dataType,
            success: function(data) {
               // alert(data);
               Service.parse(data);
               //$.extend(This.result, Service.result);
               update_callback(Service);
               if (--This.running == 0)
                  stop_callback();
            },
            error: function() {
               if (--This.running == 0)
                  stop_callback();
            }
         });
      }
   }

};
