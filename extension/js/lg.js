/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

Conetserv.LookingGlass = new Object();

/* enums for storing ids of looking glass services for selecting and starting them*/
Conetserv.LookingGlass.enums = {
  CERN_PING_V4 : 0,
  ATMAN_PING_V4 : 1,
  ILAN_PING_V4 : 2,
  SPARKLE_PING_V4: 3,

  CERN_PING_V6 : 10,
  ATMAN_PING_V6 : 11,

  CERN_TRACERT_V4 : 20,
  ATMAN_TRACERT_V4 : 21,
  ILAN_TRACERT_V4 : 22,
  SPARKLE_TRACERT_V4: 23,

  CERN_TRACERT_V6 : 30,
  ATMAN_TRACERT_V6 : 31
};

/* services parsers */
Conetserv.LookingGlass.service = new Array();

/*{
   // TEMPLATE OF SERVICE PARSER

   name: 'Name of parsed service',
   link: 'URL to service',

   id_enum: 'Enum from LookingGlass.enums',

   service: 'PING', //'TRACE'

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

/************************************** PING V4 *********************************************************/

/* PING from ILAN IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'ILAN Looking Glass',
   link: 'http://noc.ilan.net.il/LG/',

   id_enum: Conetserv.LookingGlass.enums.ILAN_PING_V4,

   stable: '2010-08-12',

   service: 'PING',

   request: [{
      type: 'GET',
      url: '',
      data: {
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.url = 'http://noc.ilan.net.il/cgi-bin/lg.sh?';
         this.url += "router=" + Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.ILAN_PING_V4];
         this.url += "&query=ping&parameter=" + Conetserv.Url.hostname;
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE>(?:\r?\n)*([^<]*)(?:\r?\n)*<\/PRE>)/;
         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");

         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]/i;
         return result;
      }
   }]
});

/* PING from CERN IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'CERN Looking glass server',
   link: 'http://lg.cern.ch/lg/',

   id_enum: Conetserv.LookingGlass.enums.CERN_PING_V4,

   stable: '2010-08-12',

   service: 'PING',

   request: [{
      type: 'POST',
      url: 'http://lg.cern.ch/lg/',
      data: {
         'query': 'ping', /* prepare */
         'protocol' : 'IPv4',
         'Submit': 1
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.data['addr'] = Conetserv.Url.hostname;
         this.data["router"] = Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.CERN_PING_V4];
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*)(?:\r?\n)*<\/CODE><\/PRE>)/;
         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]*/i;
         return result;
      }
   }]
});

/* PING from SPARKLE IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'Telecom Italia Sparkle\'s Looking Glass',
   link: 'http://gambadilegno.noc.seabone.net/lg/',

   id_enum: Conetserv.LookingGlass.enums.SPARKLE_PING_V4,

   stable: '2010-08-23',

   service: 'PING',

   request: [{
      type: 'POST',
      url: 'http://gambadilegno.noc.seabone.net/lg/lg.cgi',
      data: {
         'query': 'ping', /* prepare */
         'Submit': 1
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.data['addr'] = Conetserv.Url.hostname;
         this.data['router'] = Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.SPARKLE_PING_V4];
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE>(?:\r?\n)*([^<]*))/i;
         arr = pattern.exec(data);
         
         if (arr && arr[2])
            result = arr[2] + "</Pre>";//.replace(/\r\n/g,"<br />");
         return result;
      }
   }]
});

/* PING from ATMAN IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'ATMAN Looking glass',
   link: 'http://lg.atman.pl/',

   id_enum: Conetserv.LookingGlass.enums.ATMAN_PING_V4,

   stable: '2010-08-12',

   service: 'PING',

   request: [{
      type: 'GET',
      url: '',
      data: {
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.url = 'http://lg.atman.pl/?query=ping&protocol=IPv4&';
         this.url += "router=" + Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.ATMAN_PING_V4];
         this.url += "&addr=" + Conetserv.Url.hostname;
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*)<\/CODE><\/PRE>)/;
         var pattern2 = /(<PRE><CODE>[^]*<b>(?:\r?\n)*([^<]*)\n)/i
         var arr;

         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         else {
            arr = pattern2.exec(data);
         }
         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");

         return result;
      }
   }]
});

/************************************** PING V6 *********************************************************/
/* PING from CERN IPv6 */

Conetserv.LookingGlass.service.push({

   name: 'CERN Looking glass server',
   link: 'http://lg.cern.ch/lg/',

   id_enum: Conetserv.LookingGlass.enums.CERN_PING_V6,

   stable: '2010-08-12',

   service: 'PING6',

   request: [{
      type: 'POST',
      url: 'http://lg.cern.ch/lg/',
      data: {
         'query': 'ping', /* prepare */
         'protocol' : 'IPv6',
         'Submit': 1
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.data['addr'] = Conetserv.Url.hostname;
         this.data["router"] = Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.CERN_PING_V6];
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*)<\/CODE><\/PRE>)/;
         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]*/i;

         return result;
      }
   }]
});

/* PING from ATMAN IPv6 */

Conetserv.LookingGlass.service.push({

   name: 'ATMAN Looking glass',
   link: 'http://lg.atman.pl/',

   id_enum: Conetserv.LookingGlass.enums.ATMAN_PING_V6,

   stable: '2010-08-12',

   service: 'PING6',

   request: [{
      type: 'GET',
      url: '',
      data: {
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.url = 'http://lg.atman.pl/?query=ping&protocol=IPv6&';
         this.url += "router=" + Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.ATMAN_PING_V6];
         this.url += "&addr=" + Conetserv.Url.hostname;
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE><CODE>\n*([^<]*)\n<\/CODE><\/PRE>)/;
         var pattern2 = /(<PRE><CODE>[^]*<b>\n*([^<]*))\n/i
         var arr;

         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         else {
            arr = pattern2.exec(data);
         }
         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         
         return result;
      }
   }]
});

/************************************** TRACEROUTE V4 *********************************************************/
/* TRACEROUTE from ILAN IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'ILAN Looking Glass',
   link: 'http://noc.ilan.net.il/LG/',

   id_enum: Conetserv.LookingGlass.enums.ILAN_TRACERT_V4,

   stable: '2010-08-12',

   service: 'TRACE',

   request: [{
      type: 'GET',
      url: '',
      data: {
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.url = 'http://noc.ilan.net.il/cgi-bin/lg.sh?';
         this.url += "router=" + Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.ILAN_TRACERT_V4];
         this.url += "&query=trace&parameter=" + Conetserv.Url.hostname;
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE>(?:\r?\n)*([^<]*)<\/PRE>)/;
         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");

         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]/i;
         
         return result;
      }
   }]
});

/* TRACEROUTE from ATMAN IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'ATMAN Looking glass',
   link: 'http://lg.atman.pl/',

   id_enum: Conetserv.LookingGlass.enums.ATMAN_TRACERT_V4,

   stable: '2010-08-12',

   service: 'TRACE',

   request: [{
      type: 'GET',
      url: '',
      data: {
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.url = 'http://lg.atman.pl/?query=trace&protocol=IPv4&';
         this.url += "router=" + Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.ATMAN_TRACERT_V4];
         this.url += "&addr=" + Conetserv.Url.hostname;
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*)<\/CODE><\/PRE>)/;
         var pattern2 = /(<PRE><CODE>[^]*<b>(?:\r?\n)*([^<]*)\n)/i
         var arr;

         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         else {
            arr = pattern2.exec(data);
         }
         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");

         return result;
      }
   }]
});

/* TRACE from CERN IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'CERN Looking glass server',
   link: 'http://lg.cern.ch/lg/',

   id_enum: Conetserv.LookingGlass.enums.CERN_TRACERT_V4,

   stable: '2010-08-12',

   service: 'TRACE',

   request: [{
      type: 'POST',
      url: 'http://lg.cern.ch/lg/',
      data: {
         'query': 'trace', /* prepare */
         'protocol' : 'IPv4',
         'Submit': 1
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.data['addr'] = Conetserv.Url.hostname;
         this.data['router'] = Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.CERN_TRACERT_V4];
         return true;
      },
      parse: function(data) {
         var result = '';
         data = data.replace(/\[([^\]]*)\]\s*/g, '');
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*))/;
         var arr = data.match(pattern);
         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]*/i;

         return result;
      }
   }]
});

/* TRACEROUTE from SPARKLE IPv4 */

Conetserv.LookingGlass.service.push({

   name: 'Telecom Italia Sparkle\'s Looking Glass',
   link: 'http://gambadilegno.noc.seabone.net/lg/',

   id_enum: Conetserv.LookingGlass.enums.SPARKLE_TRACERT_V4,

   stable: '2010-08-23',

   service: 'TRACE',

   request: [{
      type: 'POST',
      url: 'http://gambadilegno.noc.seabone.net/lg/lg.cgi',
      data: {
         'query': 'trace', /* prepare */
         'Submit': 1
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.data['addr'] = Conetserv.Url.hostname;
         this.data['router'] = Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.SPARKLE_TRACERT_V4];
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE>(?:\r?\n)*([^<]*))/i;
         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2] + "</Pre>";//.replace(/\r\n/g,"<br />");
         return result;
      }
   }]
});


/************************************** TRACEROUTE V6 *********************************************************/
/* TRACEROUTE from ATMAN IPv6 */

Conetserv.LookingGlass.service.push({

   name: 'ATMAN Looking glass',
   link: 'http://lg.atman.pl/',

   id_enum: Conetserv.LookingGlass.enums.ATMAN_TRACERT_V6,

   stable: '2010-08-12',

   service: 'TRACE6',

   request: [{
      type: 'GET',
      url: '',
      data: {
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.url = 'http://lg.atman.pl/?query=trace&protocol=IPv6&';
         this.url += "router=" + Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.ATMAN_TRACERT_V6];
         this.url += "&addr=" + Conetserv.Url.hostname;
         return true;
      },
      parse: function(data) {
         var result = '';
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*)<\/CODE><\/PRE>)/i
         var pattern2 = /(<PRE><CODE>[^]*<b>(?:\r?\n)*([^<]*)\n)/i
         var arr;
         
         arr = pattern.exec(data);

         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         else {
            arr = pattern2.exec(data);
         }
         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         

         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]*/i;
         return result;
      }
   }]
});

/* TRACE from CERN IPv6 */

Conetserv.LookingGlass.service.push({

   name: 'CERN Looking glass server',
   link: 'http://lg.cern.ch/lg/',

   id_enum: Conetserv.LookingGlass.enums.CERN_TRACERT_V6,

   stable: '2010-08-12',

   service: 'TRACE6',

   request: [{
      type: 'POST',
      url: 'http://lg.cern.ch/lg/',
      data: {
         'query': 'trace', /* prepare */
         'protocol' : 'IPv6',
         'Submit': 1
      },
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {
         this.data['addr'] = Conetserv.Url.hostname;
         this.data['router'] = Conetserv.Options.ext_services_router()[Conetserv.LookingGlass.enums.CERN_TRACERT_V6];
         return true;
      },
      parse: function(data) {
         var result = '';
         data = data.replace(/\[([^\]]*)\]\s*/g, '');
         var pattern = /(<PRE><CODE>(?:\r?\n)*([^<]*))/;
         var arr = data.match(pattern);
         if (arr && arr[2])
            result = arr[2];//.replace(/\r\n/g,"<br />");
         //pattern = /descr:[\s]*([a-zA-Z-]*)[\s]*/i;

         return result;
      }
   }]
});


/* queue of services to be run */
Conetserv.LookingGlass.queue = new Array();

/* storage of services results */
Conetserv.LookingGlass.result = new Object();

/* number of running services */
Conetserv.LookingGlass.running = 0;

/* XMLHttpRequest objects returned by ajax request */
Conetserv.LookingGlass.runningServices = new Array();

/* Bool defining, if the looking glass service has been stopped manually */
Conetserv.LookingGlass.manuallyStopped = false;

/* try to run services from the queue */
Conetserv.LookingGlass.run = function() {

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

      /* Make ajax request and store object it returns */
      this.runningServices.push($.ajax({
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
            $.extend(Conetserv.LookingGlass.result, result);
            this.This.result_callback(this.This.service[this.Request.ServiceId], result);
            if (--Conetserv.LookingGlass.running == 0) {
               this.This.stopped_callback();
            }
            else
               Conetserv.LookingGlass.run();
         },
         /* error */
         error: function() {
            if (--this.This.running == 0)
               this.This.stopped_callback();
         }
      }));
   }

   this.queue = wait_queue;
};

Conetserv.LookingGlass.start = function(started_callback, result_callback, stopped_callback) {

   /* check if still running */
   if (this.queue.length > 0 || this.running > 0) {
      throw "Conetserv.LookingGlass already running..";
      return;
   }

   /* check for URL value */
   if(!Conetserv.Url.set(document.getElementById("external-url").value)) {
      document.getElementById("external-url").style.color="red";
      document.getElementById("external-url").focus();
      return;
   }
   
   this.runningServices = [];
   this.manuallyStopped = false;

   /* callback functions init, throw started */
   this.result_callback = result_callback;
   this.stopped_callback = stopped_callback;

   if (!this.result)
      this.result = {};

   /* foreach services and it's requests */
   for (var i = 0; i < this.service.length; ++i) {
      /* check, if service is allowed in options */
      if(Conetserv.Options.ext_services() && Conetserv.Options.ext_services()[this.service[i].id_enum]) {
         /* increase number of running services in externalservices object */
         switch(this.service[i].service) {
            case 'PING':
               Conetserv.ExternalServices.Ping.max++;
               break;
            case 'PING6':
               Conetserv.ExternalServices.Ping6.max++;
               break;
            case 'TRACE':
               Conetserv.ExternalServices.Tracert.max++;
               break;
            case 'TRACE6':
               Conetserv.ExternalServices.Tracert6.max++;
               break;
         }

         for (var j = 0; j < this.service[i].request.length; ++j) {
            this.service[i].result = {};
            this.service[i].request[j].ServiceId = i;
            this.queue.push(this.service[i].request[j]);
         }
      }
   }

   started_callback();

   /* try to run services from the queue */
   this.run();
};

/* Stop all running services */
Conetserv.LookingGlass.stop = function() {
   this.manuallyStopped = true;
   
   for(var key in this.runningServices) {
      this.runningServices[key].abort();
   }

   this.running = 0;
}
