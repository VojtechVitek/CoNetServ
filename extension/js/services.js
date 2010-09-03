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
   stable: '2010-08-31',

   request: [{
      type: 'GET',
      url: 'http://www.maxmind.com/app/locate_demo_ip',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      prepare: function(result) {

         if (!result.externIpv4)
            return false;

         this.data.ips = result.externIpv4;
         return true;
      },
      parse: function(data) {

         var i, result = {};

         /* Find table with data */
         if ((i = data.indexOf('<th>Hostname</th>')) == -1)
            return result;
         data = data.substr(i, data.length - i);
         if ((i = data.indexOf('<tr>')) == -1)
            return result;
         data = data.substr(i, data.length - i);
         if ((i = data.indexOf('</tr>')) == -1)
            return result;
         data = data.substr(0, i);

         /* Split data to lines */
         var lines = data.split(/\r?\n/);
         /* Line pattern */
         var pattern = /<td><font size="-1">([^<]*)<\/font><\/td>/;
         /* For each line */
         for (var i = 0; i < lines.length; ++i) {
            arr = pattern.exec(lines[i]);
            if (arr && arr[1]) {
               switch(i) {
               case 3:
                  result.countryCode = arr[1];
                  break;
               case 4:
                  result.country = arr[1];
                  break;
               case 6:
                  result.region = arr[1];
                  break;
               case 7:
                  result.city = arr[1];
                  break;
               case 9:
                  result.latitude = arr[1];
                  break;
               case 10:
                  result.longitude = arr[1];
                  break;
               case 11:
                  result.provider = arr[1];
                  break;
               default:
                  continue;
               }
            }
         }

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

         if(!data) {
            return result;
         }

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

         if (data.latitude)
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

   stable: '2010-09-03',

   request: [{
      type: 'GET',
      url: 'http://www.mojeip.cz',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {

         var result = {};
         var pattern = /<p class="date"><span> IPv4 <\/span><\/p>\s*<p class="mojeip">\s*([0-9.:]+)\s*<\/p>/;
         var arr = pattern.exec(data);

         if (arr && arr[1])
            result.externIpv4 = arr[1];

         pattern = /<p class="date"><span>host<\/span><\/p>\s*<p class="mojeip">\s*([a-z0-9.]+)\s*<\/p>/;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.hostname = arr[1];

         pattern = /<p class="date"><span> IPv6<\/span><\/p>\s*<p class="aside-padding smaller box">\s*([a-z0-9.]+)\s*<\/p>/;
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

   stable: '2010-03-24',

   request: [{
      type: 'GET',
      url: 'http://ipinfo.info/html/privacy-check.php',
      data: {},
      dataType: 'text',
      dataCharset: 'UTF-8',
      parse: function(data) {
         var result = {};
         var pattern = /<p class="nomargins"><span style=".*?"><B>[\s]*((?:\d{1,3}\.){3}\d{1,3})<\/B><\/span>&nbsp;<\/p>/;
         var arr = pattern.exec(data);

         if (arr && arr[1])
            result.externIpv4 = arr[1];

         pattern = /<p class="nomargins"><span style=".*?">[\s]*([a-z0-9.]*)[\s]*<\/span>&nbsp;<\/p>/;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.hostname = arr[1];

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
         var pattern = /route:[\s]*((?:\d{1,3}\.){3}\d{1,3}\/[0-9]{1,2})/;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.route = arr[1];

         pattern = /descr:[\s]*([a-zA-Z-]*)[\s]*/i;
         arr = pattern.exec(data);

         if (arr && arr[1])
            result.provider = arr[1];

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

   if (true)
   console.log(Conetserv.Services.service[4].request[0].parse("\n\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"cs\" lang=\"cs\">\n<head>\n    <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n    <meta http-equiv=\"content-language\" content=\"cs\" />\n    <meta name=\"robots\" content=\"all,follow\" />\n\n    <meta name=\"author\" lang=\"cs\" content=\"Moje IP adresa, zjištění IP adresy [www.mojeip.com]\" />\n     <link rel=\"shortcut icon\" href=\"./favicon.ico\" />\n\n\n    <meta name=\"description\" content=\"Moje IP adresa je intenretový server k zjištění IP adresy, rychlosti připojení k internetu, pingu, operačního sytému, verze prohlížeče a spousty dalších užitečných věcí.\" />\n    <meta name=\"keywords\" content=\"moje ip adresa, zjištění ip adresy, zjištění mac adresy, měření rychlosti připojení\" />\n\n    <link rel=\"stylesheet\" media=\"screen,projection\" type=\"text/css\" href=\"css/main.css\" />\n    <!--[if lte IE 6]><link rel=\"stylesheet\" type=\"text/css\" href=\"css/main-msie.css\" /><![endif]-->\n    <link rel=\"stylesheet\" media=\"screen,projection\" type=\"text/css\" href=\"css/scheme.css\" />\n    <link rel=\"stylesheet\" media=\"print\" type=\"text/css\" href=\"css/print.css\" />\n\n    <title>Moje IP adresa , zjištění IP adresy, jak zjistit IP adresu | www.mojeip.cz</title>\n</head>\n\n<body>\n\n<!-- Header -->\n<div id=\"header\" class=\"box\">\n    <div class=\"main\">\n\n        <!-- Your logo -->\n        <h1 id=\"logo\"><a href=\"http://www.mojeip.cz/\" title=\"Moje IP adresa, zjištění IP adresy, jak zjistit ip adresu\">moje <span>ip </span> adresa &nbsp; &nbsp; zjištění<span> ip </span> adresy</a></h1>\n        <hr class=\"noscreen\" />\n\n        <!-- Hidden navigation -->\n        <p class=\"noscreen noprint\"><em>Quick links: <a href=\"#content\">content</a>, <a href=\"#nav\">navigation</a>, <a href=\"#search\">search</a>.</em></p>\n\n        <!-- Search -->\n\n        <fieldset id=\"search\">\n            <form action=\"#\" method=\"get\">\n                <div>\n                    <span class=\"noscreen\">Fulltext:</span>\n                    <div id=\"search-input\"><input type=\"text\" size=\"30\" name=\"\" /></div>\n                    <input type=\"image\" src=\"design/inp-search-submit.gif\" value=\"Search\" name=\"\" id=\"search-submit\" />\n                </div>\n            </form>\n        </fieldset>\n\n\n    </div> <!-- /main -->\n</div> <!-- /header -->\n\n<div class=\"main bg box\">\n\n    <!-- Navigation -->\n\n\n    <div id=\"nav\" class=\"box\">\n\n        <h3 class=\"noscreen\">Navigation</h3>\n         <ul>\n\n            <li id=\"nav-active\"><a href=\"http://www.mojeip.cz/\" title=\"moje ip adresa, zjištění ip adresy\">moje ip adresa</a></li>\n            <li ><a href=\"http://mereni-rychlosti-pripojeni-k-internetu.mojeip.cz/\" title=\"měření rychlosti připojení k internetu\">měření rychlosti připojení k internetu</a></li>\n            <li ><a href=\"http://ping.mojeip.cz/\" title=\"ping\">ping</a></li>\n            <li ><a href=\"http://anonymizer.mojeip.cz/\" title=\"anonymizér\">anonymizér</a></li>\n            <li ><a href=\"http://blog.mojeip.cz/\" title=\"blog.mojeip.cz\">blog</a></li>\n            <li ><a href=\"http://blog.mojeip.cz/forum.p18.html\" title=\"forum.mojeip.cz\">fórum</a></li>\n        </ul>\n\n    <hr class=\"noscreen\" />\n    </div> <!-- /nav -->\n\n    <!-- 2 columns (Content + Sidebar) -->\n    <div class=\"box\">\n\n        <!-- Content -->\n        <div id=\"content\">\n\n\n\n               <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"><a href=\"#ipv4\" class=\"ico-comment\">více ...</a></p>\n                    <h3>moje IP adresa (IPv4)</h3>\n                </div>\n                <p class=\"date\"><span> IPv4 </span></p>\n\n                <p class=\"mojeip\">\n\n\n\n\n88.146.86.253\n    </p>\n\n\n\n\n\n\n\n\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n            <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"><a href=\"#hostname\" class=\"ico-comment\">více ...</a></p>\n                    <h3>vaše hostname</h3>\n                </div>\n                <p class=\"date\"><span>host</span></p>\n                <p class=\"mojeip\">\n\n\n                &nbsp;a40-brn1-6-253.static.adsl.vol.cz\n\n\n                </p>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n\n\n                            <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n\n                    <h3>reklama</h3>\n                </div>\n\n                <p><center>\n              <script type=\"text/javascript\"><!--\ngoogle_ad_client = \"pub-5788244663422315\";\n/* mojeip.cz HP 468x60, vytvořeno 16.8.10 */\ngoogle_ad_slot = \"3779391134\";\ngoogle_ad_width = 468;\ngoogle_ad_height = 60;\n//-->\n</script>\n<script type=\"text/javascript\"\nsrc=\"http://pagead2.googlesyndication.com/pagead/show_ads.js\">\n</script>  </center>\n</p>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n\n\n\n\n\n\n\n                  <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"><a href=\"#local_IP_adresa\" class=\"ico-comment\">více ...</a></p>\n                    <h3>vaše interní IP adresa</h3>\n                </div>\n                <p class=\"date\"><span>local</span></p>\n                <p>\n                <iframe src=\"http://www.mojeip.cz/new2/ip_include.php\" frameborder=\"0\" scrolling=\"no\" width=\"520\" height=\"44px\"></iframe>\n</p>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n\n\n\n            <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"><a href=\"#ipv6\" class=\"ico-comment\">více ...</a></p>\n                    <h3>moje IP adresa (IPv6)</h3>\n                </div>\n                <p class=\"date\"><span> IPv6</span></p>\n                <p class=\"aside-padding smaller box\">\n\n\n\n                Tvoje IP je <b>88.146.86.253</b>  a patří do protokolu : IPv4<br /> IPv6 compression : ::1\nn<br />IPv6 Uncompression : 0:0:0:0:0:0:0:1\nn<br />\n\n\n\n\n\n\n\n                </p>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n\n                  <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"></p>\n                    <h3>další informace (OS, prohlížeč, JavaScript, cookoes, atd...)</h3>\n                </div>\n                <p class=\"date\"><span>info</span></p>\n                <p >\n\n<table border=\"0\" width=\"570\" valign=top class=\"smaller box\">\n<script type=\"text/javascript\" src=\"inc/javascript_cookies.js\"></script>\n\n<script type=\"text/javascript\">\n/*\nScript Name: Your Computer Information\nAuthor: Harald Hope, Website: http://TechPatterns.com/\nScript Source URI: http://TechPatterns.com/downloads/browser_detection.php\nVersion: 1.2.4\nCopyright (C) 3 April 2010\n\nThis program is free software; you can redistribute it and/or modify it under\nthe terms of the GNU General Public License as published by the Free Software\nFoundation; either version 3 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT\nANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\nFOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\nGet the full text of the GPL here: http://www.gnu.org/licenses/gpl.txt\n\nThis script requires the Full Featured Browser Detection and the Javascript Cookies scripts\nto function.\nYou can download them here.\nhttp://TechPatterns.com/downloads/browser_detection_php_ar.txt\nhttp://TechPatterns.com/downloads/javascript_cookies.txt\n\nPlease note: this version requires the php browser_detection script version 5.3.3 or\nnewer, because of the new full return of arrays $moz_array and $webkit_array as keys\n10 and 11, and $webkit_array key 7, and use of the new array key 14, true_msie_version.\n*/\n\n/*\nIf your page is XHMTL 1 strict, you have to\nput this code into a js library file or your\npage will not validate\n*/\nfunction client_data(info)\n{\n   if (info == 'width')\n   {\n      width_height_html = '<h4  class=\"right-bar\">Rozlišení obrazovky</h4>';\n      width = (screen.width) ? screen.width:'';\n      height = (screen.height) ? screen.height:'';\n      // check for windows off standard dpi screen res\n      if (typeof(screen.deviceXDPI) == 'number') {\n         width *= screen.deviceXDPI/screen.logicalXDPI;\n         height *= screen.deviceYDPI/screen.logicalYDPI;\n      }\n      width_height_html += '<p class=\"right-bar\">' + width + \" x \" +\n         height + \" pixels</p>\";\n      (width && height) ? document.write(width_height_html):'';\n   }\n   else if (info == 'js' )\n   {\n      document.write('<p class=\"right-bar\">JavaScript je zapnut.</p>');\n   }\n   else if ( info == 'cookies' )\n   {\n      expires ='';\n      Set_Cookie( 'cookie_test', 'it_worked' , expires, '', '', '' );\n      string = '<h4  class=\"right-bar\">Cookies</h4><p class=\"right-bar\">';\n      if ( Get_Cookie( 'cookie_test' ) )\n      {\n         string += 'Cookies jsou povoleny</p>';\n      }\n      else {\n         string += 'Cookies jsou zakazány</p>';\n      }\n      document.write( string );\n   }\n}\n</script>\n\n\n\n<TR  valign=top>\n<TD width=\"33%\"  valign=top>\n\n<div class=\"float-left-01\">\n         <h4 class=\"right-bar\">Operační systém:</h4><p class=\"right-bar\">Linux<br /> Smart Move!!!</p><h4 class=\"right-bar\">Prohlížeč / UA:</h4><p class=\"right-bar\">User Agent: Chrome 6.0.486.0<br />Engine: AppleWebKit v: 534.5</p>         </TD>\n<TD width=\"33%\"  valign=top>\n\n         <script type=\"text/javascript\">\n            client_data('width');\n         </script>\n\n         </TD>\n<TD width=\"33%\" valign=top>\n\n         <h4 class=\"right-bar\">JavaScript</h4>\n         <script type=\"text/javascript\">\n            client_data('js');\n         </script>\n         <noscript>\n         <p class=\"right-bar\">JavaScript je vypnutý</p>\n         </noscript>\n         <script type=\"text/javascript\">\n            client_data('cookies');\n         </script>\n   </div>\n\n\n   </TD>\n</TR></table>\n\n</p>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n                            <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"></p>\n                    <h3>nejčtenější články</h3>\n                </div>\n                <p class=\"date\"><span>top</span></p>\n   <ul class=\"ul-list-02 box\">\n                    <li><span class=\"f-right\">vydáno: 13.10.2009 0:49  </span> <a href=\"http://blog.mojeip.cz/zjisteni-mac-adresy.a5.html\">Zjištění MAC adresy</a></li>\n                    <li><span class=\"f-right\">vydáno: 20.2.2010 21:19 </span> <a href=\"http://blog.mojeip.cz/zmena-mac-adresy-nejen-u-sitove-karty.a13.html\">Změna MAC adresy</a></li>\n                    <li><span class=\"f-right\">vydáno: 22.11.2009 6:42  </span> <a href=\"http://blog.mojeip.cz/ip-kamery-nejen-pro-zabavu.a12.html\">IP kamery - nejen pro zábavu</a></li>\n\n                </ul>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n                        <!-- Article -->\n            <div class=\"article\">\n\n                <div class=\"info box\">\n                    <p class=\"f-right\"></p>\n                    <h3>nejčastěji kladené dotazy</h3>\n                </div>\n                <p class=\"date\"><span>faq</span></p>\n                <p class=\"smaller box\">\n             <b>Co je to IP adresa?</b><br><a name=\"ipv4\"></a>\nIP adresa je jedinečná adresa počítače v internetu. Udává se ve tvaru yyy.yyy.yyy.yyy, kde yyy je číslo v rozsahu 0 až 255. Může vypadat například takto:\n 93.112.07.247 .\n<br><br>\n<b>Co je to pevná IP?</b><br>\nTato IP adresa zůstává po celou dobu dané služby stejná a nemění se.Adresu přiděluje administrátor(většinou ISP) a tato adresa počítači zůstává i po vypnutí\n nebo restartu modemu či systému.\n<br><br>\n<b>Co je to hostname?</b><br><a name=\"hostname\"></a>\n\nHostname, slouží podobně jako IP adresa k idetifikaci počítače v síti.<br><br>\n\n<b>Co je to ISP?</b><br>\n\nInternet Service Provider (ISP) - poskytovatel služeb přístupu k internetu.<br><br>\n<b>Co je to TCP/IP?</b><br>\n\n\nTransmission Control Protocol/Internet Protocol - v současné době nejpoužívanější protokol(chcete-li standard) pro komunikaci v sítích.V 70 letech vyvinutý pro\nministerstvo obrany USA.\n<br><br>\n<b>Co je IPv6 adresa?</b><a name=\"ipv6\"></a><br>\nIPv6 (internetový protokol verze 6) v současné době nahrazuje nevyhovující IPv4. Protokol IPv6 hlavně poskytne mnohem větší prostor adres, které v protokolu IPv4 docházejí. Dalším plusem tohoto moderního protokolu je lepší \"práce\"  s vysokorychlostními daty.\n<br><br>\n\n\n\n\n\n\n\n\n<b>Co je to interní IP adresa?</b><br>\nInterní nebo i lokální IP adresa je provozována v Local Area Network (taky LAN, lokální síť, místní síť). Lokální adresy se používají pouze v těchto malých sítích, které jsou převážně používany v domáctnostech, školách či malých a středních firmách.\n<br><br>\n\n\n<b>Chci vědět o IP adrese více!?</b><br><a name=\"local_IP_adresa\"></a>\nPak navštivte největší Internetovou encyklopedii\n<a href=\"http://cs.wikipedia.org/wiki/IP_adresa\" target=\"_blank\" title=\"O IP adrese na Wikipedia \">wikipedia</a>\n\n<br><br>\n\n\n<strong>Jak zjistit IP adresu</strong> ?<br><a name=\"\"></a>\nJednoduše, přece zde:-) na\n<a href=\"http://www.mojeip.cz/\" target=\"\" title=\"Jak zjistit IP adresu\">www.mojeip.cz</a> .\n\n</p>\n\n            <hr class=\"noscreen\" />\n            </div> <!-- /article -->\n\n\n\n        </div> <!-- /content -->\n\n        <hr class=\"noscreen\" />\n\n        <!-- Sidebar -->\n        <div id=\"aside\">\n\n            <!-- About me -->\n\n\n            <h3 class=\"title\">facebook </h3>\n\n            <div class=\"aside-padding smaller box\">\n            <br>\n            <iframe src=\"http://www.facebook.com/plugins/likebox.php?id=142628949099918&amp;width=190&amp;connections=0&amp;stream=true&amp;header=false&amp;height=305\" scrolling=\"no\" frameborder=\"0\" style=\"border:none; overflow:hidden; width:190px; height:305px;\" allowTransparency=\"true\"></iframe>\n\n\n            </div>\n\n\n\n\n\n\n\n\n\n\n\n             <!-- /aside-padding -->\n\n            <!-- Archive -->\n\n            <h3 class=\"title\">poslední přidané články</h3>\n\n            <div class=\"aside-padding smaller box\">\n                        <br>\n                  <a href=\"http://blog.mojeip.cz/opet-v-novem-kabate-ii.a17.html\" class=\"rsslink\" rel=\"nofollow\"  target=\"_blank\">Opět v novém kabátě II.</a><br />Opět a téměř po roce přicházíme s úpravami vzhledu a rozložením kategorií serveru. <br/>\n   <a href=\"http://blog.mojeip.cz/upc-fiber-power-25.a16.html\" class=\"rsslink\" rel=\"nofollow\"  target=\"_blank\">UPC Fiber Power 25</a><br />UPC ode dneška obohacuje svou nabídku superrychlého internetu známou pod názvem Fiber Power o další atraktivní službu: Fiber Power 25. Fiber Power 25 dosahuje rychlost 25 Mbit/s v příchozím (download) a 1,5 Mbit/s v odchozím (upload) směru. Stojí 599 Kč měsíčně včetně DPH. <br/>\n   <a href=\"http://blog.mojeip.cz/nickelodeon.a15.html\" class=\"rsslink\" rel=\"nofollow\"  target=\"_blank\">Nickelodeon</a><br />Praha 6. dubna 2010. UPC opět rozšiřuje nabídku své digitální televize o novou stanici, tentokrát o Nickelodeon. Nickelodeon se zaměřuje na děti a mládež a vysílá dvacet čtyři hodiny denně, sedm dní v týdnu, v českém jazyce. <br/>\n   <a href=\"http://blog.mojeip.cz/firefox-3.6.3.a8.html\" class=\"rsslink\" rel=\"nofollow\"  target=\"_blank\">Firefox 3.6.3</a><br />Mozilla vydala rychlou opravu pro kritickou zranitelnost prohlížeče Firefox, kterou německý vývojář Nils Maier použil ke svému útoku na soutěži Pwn2Own. <br/>\n   <a href=\"http://blog.mojeip.cz/zmena-mac-adresy-nejen-u-sitove-karty.a13.html\" class=\"rsslink\" rel=\"nofollow\"  target=\"_blank\">Změna MAC adresy nejen u síťové karty</a><br />Někdy je potřeba nejenom zjistit, ale i změnit MAC adresu některých zařízení, převážně síťové karty. <br/>\n.\n.\n.\n\n\n            </div>\n            <!-- /aside-padding -->\n\n            <!-- RSS Feeds -->\n            <h3 class=\"title\">buď v kontaktu</h3>\n\n             <ul id=\"ul-rss\">\n                <li><img src=\"http://www.mojeip.cz/new2/design/facebook.png\" alt=\"alt\" title=\"dsad\" align=\"absmiddle\" border=\"0\"> <a href=\"http://www.facebook.com/pages/mojeIPcz/142628949099918\"> Facebook</a></li>\n                    <li><img src=\"http://www.mojeip.cz/new2/design/ico-rss.gif\" alt=\"alt\" title=\"dsad\" align=\"absmiddle\" border=\"0\"> <a href=\"http://blog.mojeip.cz/remote/rss.php?tp=4&id=9\"> RSS blogu</a></li>\n            </ul>\n\n\n\n        </div> <!-- /aside -->\n\n    </div> <!-- /box -->\n\n    <hr class=\"noscreen\" />\n\n    <!-- Footer -->\n\n    <div id=\"footer\" class=\"box\">\n\n        <!-- Do you want remove this backlinks? Look at www.nuviotemplates.com/payment.php\n        <p class=\"f-right\"><a href=\"http://www.nuviotemplates.com/\">Free web templates</a> presented by <a href=\"http://www.qartin.cz/\">QARTIN</a>, <a href=\"http://www.cssrevue.com/\">CSS Gallery</a></p>\n    Do you want remove this backlinks? Look at www.nuviotemplates.com/payment.php -->\n\n        <p>&copy;&nbsp;2010 <a href=\"./\">moje ip adresa</a></p>\n    <script type=\"text/javascript\">\n\n  var _gaq = _gaq || [];\n  _gaq.push(['_setAccount', 'UA-3040950-1']);\n  _gaq.push(['_setDomainName', '.mojeip.cz']);\n  _gaq.push(['_trackPageview']);\n\n  (function() {\n    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;\n    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';\n    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);\n  })();\n\n</script>\n\n\n\n    </div> <!-- /footer -->\n\n</div> <!-- /main -->\n\n</body>\n</html>\n"));
   else
   /* try to run services from the queue */
   this.run();
};
