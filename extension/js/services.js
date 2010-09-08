/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

Conetserv.Services = new Object();

/* services parsers */
Conetserv.Services.service = new Array();

/*{
   // TEMPLATE OF SERVICE PARSER

   name: 'Name of parsed service',
   link: 'URL to service',

   stable: 'YYYY-mm-dd', // date created (stable version)

   result: { // list of results (should follow common result-naming convention)
      result1: null,
      result2: null
   },

   request: [{

      // first request
      type: 'GET', // 'POST'
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
      type: 'GET',
      url: 'http://another-request-url.example.com/',
      data: {};
      dataType: 'text', // 'json', 'html',..
      dataCharset: 'UTF-8',
      // Optional function to prepare request data
      prepare: function(result) {
         if (!result.externIpv4)
            return false;
         this.data.ip = result.externIpv4;
         return true;
      },
      parse: function(data) {
         var result = {};
         // parse data to result
         return result;
      }
   }]

};
*/

Conetserv.Services.service.push({

   name: 'Google.com - JSAPI',
   link: 'http://www.google.com/',

   stable: '2010-04-06',

   request: [{
      type: 'GET',
      url: 'http://www.google.com/jsapi',
      data: {},
      dataType: 'script',
      dataCharset: 'UTF-8',
      parse: function() {
         var result = {};

         if (window.google && google.loader && google.loader.ClientLocation) {

            if (google.loader.ClientLocation.address) {
               if (google.loader.ClientLocation.address.country_code)
                  result.countryCode = google.loader.ClientLocation.address.country_code;

               if (google.loader.ClientLocation.address.country && google.loader.ClientLocation.address.country != '-' && google.loader.ClientLocation.address.country != 'NaN')
                  result.country = google.loader.ClientLocation.address.country;

               if (google.loader.ClientLocation.address.city && google.loader.ClientLocation.address.city != '-' && google.loader.ClientLocation.address.city != 'NaN')
                  result.city = google.loader.ClientLocation.address.city;

               if (google.loader.ClientLocation.address.region && google.loader.ClientLocation.address.region != '-' && google.loader.ClientLocation.address.region != 'NaN')
                  result.region = google.loader.ClientLocation.address.region;
            }

            if (google.loader.ClientLocation.latitude)
               result.latitude = google.loader.ClientLocation.latitude;

            if (google.loader.ClientLocation.latitude)
               result.longitude = google.loader.ClientLocation.longitude;

            result.zoom = 10;
         }

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'MaxMind GeoIP',
   link: 'http://www.maxmind.com/',
   stable: '2010-09-08',

   request: [{
      type: 'GET',
      url: 'http://j.maxmind.com/app/geoip.js',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};
         var pattern;
         var arr;

         pattern = /.*country_code.*?[']([^']+).*/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.countryCode = arr[1];

         pattern = /.*country_name.*?[']([^']+).*/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.country = arr[1];

         pattern = /.*city.*?[']([^']+).*/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.city = arr[1];

         pattern = /.*region_name.*?[']([^']+).*/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.region = arr[1];

         pattern = /.*latitude.*?[']([^']+).*/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.latitude = arr[1];

         pattern = /.*longitude.*?[']([^']+).*/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.longitude = arr[1];

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'IP Info DB',
   link: 'http://ipinfodb.com/',

   stable: '2010-09-08',

   request: [{
      type: 'GET',
      url: 'http://ipinfodb.com/ip_query.php',
      data: {
         ip: false,
         output: 'json',
         timezone: 'false'
      },
      dataType: 'json',
      dataCharset: 'UTF-8',
      prepare: function(result) {

         if (!result.externIpv4)
            return false;

         this.data.ip = result.externIpv4;
         return true;
      },
      parse: function(data) {

         var result = {};

         if (!data)
            return result;

         if (data.CountryCode)
            result.countryCode = data.CountryCode;

         if (data.CountryName)
            result.country = data.CountryName;

         if (data.City)
            result.city = data.City;

         if (data.RegionName)
            result.region = data.RegionName;

         if (data.Latitude)
            result.latitude = data.Latitude;

         if (data.Longitude)
            result.longitude = data.Longitude;

         return result;
      }
   }]

});


Conetserv.Services.service.push({

   name: 'Free IP geolocation webservice by Alexandre Fiori',
   link: 'http://freegeoip.appspot.com/',

   stable: '2010-09-08',

   request: [{
      type: 'GET',
      _url: 'http://freegeoip.appspot.com/json/',
      url: false /* prepare */,
      data: {},
      dataType: 'json',
      dataCharset: 'UTF-8',
      prepare: function(result) {

         if (!result.externIpv4)
            return false;

         this.url = this._url + result.externIpv4;
         return true;
      },
      parse: function(data) {

         var result = {};

         if (!data)
            return result;

         if (data.countrycode)
            result.countryCode = data.countrycode;

         if (data.countryname)
            result.country = data.countryname;

         if (data.city)
            result.city = data.city;

         if (data.regionname)
            result.region = data.regionname;

         if (data.latitude)
            result.latitude = data.latitude;

         if (data.longitude)
            result.longitude = data.longitude;

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'WIPmania.com - WorldIP API',
   link: 'http://www.wipmania.com/',

   stable: '2010-03-23',

   request: [{
      type: 'GET',
      url: 'http://api.wipmania.com/',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};
         var pattern = /^((?:\d{1,3}\.){3}\d{1,3})<br>([a-z]{1,3})$/;
         var arr = pattern.exec(data);

         if (arr && arr[1])
            result.externIpv4 = arr[1];

         if (arr && arr[2])
            result.countryCode = arr[2];

         return result;
      }
   }, {
      type: 'GET',
      url: 'http://api.wipmania.com/json',
      data: {},
      dataType: 'json',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};

         if (!data)
            return result;

         if (data.address) {
            if (data.address.country_code && data.address.country_code != '-' && data.address.country_code != 'NaN')
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

         if (data.longitude)
            result.longitude = data.longitude;

         if (data.zoom)
            result.zoom = data.zoom;

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'CGI script by Chris F.A. Johnson',
   link: 'http://cfaj.freeshell.org/',

   stable: '2010-03-23',

   request: [{
      type: 'GET',
      url: 'http://cfaj.freeshell.org/ipaddr.cgi',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};
         var pattern = /^((?:\d{1,3}\.){3}\d{1,3})/;
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
      type: 'GET',
      url: 'http://checkip.dyndns.com:8245/',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};
         var pattern = /^((?:\d{1,3}\.){3}\d{1,3})/;
         var arr = pattern.exec(data);

         if (arr && arr[1])
            result.externIpv4 = arr[1];

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'MojeIP.cz',
   link: 'http://www.mojeip.cz/',

   stable: '2010-09-08',

   request: [{
      type: 'GET',
      url: 'http://www.mojeip.cz',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};
         var pattern;
         var arr;

         pattern = /<p class="date"><span> IPv4 <\/span><\/p>\s*<p class="mojeip">\s*([0-9.:]+)\s*<\/p>/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.externIpv4 = arr[1];

         pattern = /<p class="date"><span>host<\/span><\/p>\s*<p class="mojeip">\s*(?:[&]nbsp[;])?([a-z0-9.-]+)\s*<\/p>/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.hostname = arr[1];

         pattern = /<p class="date"><span> IPv6<\/span><\/p>\s*<p class="aside-padding smaller box">\s*.*?<b>([0-9:]+)<\/b>.*?IPv6<br \/>/;
         arr = pattern.exec(data);
         if (arr && arr[1])
            result.externIpv6 = arr[1];

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'IPinfo Security Portal',
   link: 'http://ipinfo.info/',

   stable: '2010-09-08',

   request: [{
      type: 'GET',
      url: 'http://ipinfo.info/ipn/',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {
         var result = {};
         var pattern;
         var arr;

         pattern = /<h4>([0-9.]+)<br>([a-z0-9-.]*)<\/h4>/;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.externIpv4 = arr[1];

         if (arr && arr[2])
            result.hostname = arr[2];

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: 'RADb, Merit Network Inc.',
   link: 'http://www.radb.net/',

   stable: '2010-03-25',

   request: [{
      type: 'GET',
      url: 'http://www.radb.net/cgi-bin/radb/advanced-query.cgi',
      data: {
         'searchstr': false, /* prepare */
         'submit': 'Query'
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {

         if (!result.externIpv4)
            return false;

         this.data.searchstr = result.externIpv4;
         return true;
      },
      parse: function(data) {

         var result = {};
         var pattern = /route:\s*((?:\d{1,3}\.){3}\d{1,3}\/[0-9]{1,2})/;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.route = arr[1];

         pattern = /descr:\s*([a-zA-Z-]*)\s*/i;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.provider = arr[1];

         return result;
      }
   }]

});

Conetserv.Services.service.push({

   name: '2 Privacy.com',
   link: 'http://www.2privacy.com/',

   stable: '2010-09-09',

   request: [{
      type: 'GET',
      url: 'http://www.2privacy.com/www/what-is-my-ip.html',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {
         var result = {};
         var pattern;
         var arr;

         pattern = /Your IP: <font[^>]*><b>([0-9.]+)<\/b><\/font>, your Host: <font[^>]*><b>([a-z0-9.-]+)<\/b><\/font>, your Country: <font[^>]*><b>([A-Z]{2,3})/;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.externIpv4 = arr[1];

         if (arr && arr[2])
            result.hostname = arr[2];

         if (arr && arr[3])
            result.countryCode = arr[3];

         return result;
      }
   }]

});

/* queue of services to be run */
Conetserv.Services.queue = new Array();

/* storage of services results */
Conetserv.Services.result = new Object();

/* number of running services */
Conetserv.Services.running = 0;

/* try to run services from the queue */
Conetserv.Services.run = function() {

   var wait_queue = [];

   /* context */
   var Request;
   while ((Request = this.queue.shift())) {

      /* Skip services, which need some data (externIpv4 etc.)
         * to make a query, but these data are not yet available
         */
      if (Request.prepare && !Request.prepare(this.result)) {
         wait_queue.push(Request);
         continue;
      }

      ++this.running;

      /* Make ajax request */
      $.ajax({
         /* context */
         This: this,
         Request: Request,
         /* ajax settings */
         type: Request.type,
         url: Request.url,
         data: Request.data,
         dataType: Request.dataType,
         /* success */
         success: function(data) {
            var result = this.Request.parse(data);
            $.extend(Conetserv.Services.result, result);
            this.This.result_callback(this.This.service[this.Request.ServiceId], result);
            if (--this.This.running == 0)
               this.This.stopped_callback();
            else
               Conetserv.Services.run();
         },
         /* error */
         error: function() {
            if (--this.This.running == 0)
               this.This.stopped_callback();
         }
      });
   }

   this.queue = wait_queue;
};

Conetserv.Services.start = function(started_callback, result_callback, stopped_callback) {

   /* check if still running */
   if (this.queue.length > 0 || this.running > 0) {
      throw "Conetserv.Services already running..";
      return;
   }

   /* callback functions init, throw started */
   started_callback();
   this.result_callback = result_callback;
   this.stopped_callback = stopped_callback;

   if (!this.result)
      this.result = {};

   /* foreach services and it's requests */
   for (var i = 0; i < this.service.length; ++i) {
      for (var j = 0; j < this.service[i].request.length; ++j) {
         if (!this.service[i].result)
            this.service[i].result = {};
         this.service[i].request[j].ServiceId = i;
         this.queue.push(this.service[i].request[j]);
      }
   }

   /* try to run services from the queue */
   this.run();
};
