var Services = {

   service: [/*{

         // TEMPLATE SERVICE PARSER

         name: 'Name of parsed service',
         link: 'URL to service',

         stable: 'YYYY-mm-dd', // date created (stable version)

         result: { // list of results (should follow common result-naming convention)
            result1: null,
            result2: null
         },

         request: [{

            // first request
            method: 'GET', // 'POST'
            url: 'http://request-url.example.com/',
            data: {
               query: 'string',
               lang: 'en'
            },
            dataType: 'text', // 'json', 'html',..
            dataCharset: 'UTF-8',
            parse: function(data) {
               // PARSE data
               result.result1 = ...;
               result.result2 = ...;

            }, {

            // optional second request
            // ! to be called after the first request succeed
            method: 'GET',
            url: 'http://another-request-url.example.com/',
            data: {};
            dataType: 'text', // 'json', 'html',..
            dataCharset: 'UTF-8',
            prepare: function(result) {
               // Optional function to prepare request data
               this.data.ip = result.externIpv4;
            },
            parse: function(data) {
               ...
               ...
            }
         }]

   },*/ {

         name: 'WIPmania.com - WorldIP API',
         link: 'http://www.wipmania.com/',

         stable: '2010-03-23',

         request: [{
            method: 'GET',
            url: 'http://api.wipmania.com/',
            data: {},
            dataType: 'text',
            dataCharset: 'UTF-8',
            parse: function(data) {

               var result = {};
               var pattern = /^((?:\d{1,3}\.){3}\d{1,3})<br>([a-z]{1,3})$/gi;
               var arr = pattern.exec(data);

               if (arr && arr[1])
                  result.externIpv4 = arr[1];

               if (arr && arr[2])
                  result.countryCode = arr[2];

               return result;
            }
         }, {
            method: 'GET',
            url: 'http://api.wipmania.com/json',
            data: {},
            dataType: 'json',
            dataCharset: 'UTF-8',
            parse: function(data) {

               var result = {};

               if (data.address) {
                  if (data.address.country_code)
                     result.countryCode = data.address.country_code;

                  if (data.address.country)
                     result.country = data.address.country;

                  if (data.address.city && data.address.city != '-' && data.address.city != 'NaN')
                     result.city = data.address.city;

                  if (data.address.region && data.address.region != '-' && data.address.region != 'NaN')
                     result.region = data.address.region;
               }

               if (data.latitude)
                  result.latitude = data.latitude;

               if (data.latitude)
                  result.longitude = data.longitude;

               //if (data.zoom)
               //   result.zoom = data.zoom;

               return result;
            }
         }]
            
   }, {

         name: 'CGI script by Chris F.A. Johnson',
         link: 'http://cfaj.freeshell.org/',

         stable: '2010-03-23',

         request: [{
            method: 'GET',
            url: 'http://cfaj.freeshell.org/ipaddr.cgi',
            data: {},
            dataType: 'text',
            dataCharset: 'UTF-8',
            parse: function(data) {

               var result = {};
               var pattern = /^((?:\d{1,3}\.){3}\d{1,3})/gi;
               var arr = pattern.exec(data);

               if (arr && arr[1])
                  result.externIpv4 = arr[1];

               return result;
            }
         }]

   }, {

         name: 'Check IP by DynDNS.com',
         link: 'http://www.dyndns.com/',

         stable: '2010-03-23',

         request: [{
            method: 'GET',
            url: 'http://checkip.dyndns.com:8245/',
            data: {},
            dataType: 'text',
            dataCharset: 'UTF-8',
            parse: function(data) {

               var result = {};
               var pattern = /^((?:\d{1,3}\.){3}\d{1,3})/gi;
               var arr = pattern.exec(data);

               if (arr && arr[1])
                  result.externIpv4 = arr[1];

               return result;
            }
         }]

   }],

   running: 0,

   run: function(Service) {
         if (!Service.pos)
            Service.pos = 0;
         if (!Service.result)
            Service.result = {};
         var This = this;
         $.ajax({
            /* context */
            This: This,
            Service: Service,
            /* ajax settings */
            type: Service.request[Service.pos].method,
            url: Service.request[Service.pos].url,
            processData: Service.request[Service.pos].data,
            dataType: Service.request[Service.pos].dataType,
            /* success */
            success: function(data) {
               if (this.Service.request[this.Service.pos].prepare)
                  this.Service.request[this.Service.pos].prepare(this.Service.result);
               $.extend(this.Service.result, this.Service.request[this.Service.pos].parse(data));
               if (++this.Service.pos < this.Service.request.length) {
                  this.This.run(this.Service);
               } else {
                  this.This.result_callback(this.Service);
                  if (--this.This.running == 0)
                     this.This.stopped_callback();
               }
            },
            /* error */
            error: function() {
               if (--this.This.running == 0)
                  this.This.stopped_callback();
            }
         });
   },

   start: function(started_callback, result_callback, stopped_callback) {

      if (this.running == 0)
         this.running = this.service.length;
      else
         throw "Services already running..";

      started_callback();
      this.result_callback = result_callback;
      this.stopped_callback = stopped_callback;

      /* Run all services */
      for (var i = 0; i < this.service.length; ++i)
         this.run(this.service[i]);
   }

};
