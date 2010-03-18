var externIpServices = [];

externIpServices.push({

   name: 'Check IP by DynDNS.com',

   url: 'http://checkip.dyndns.com:8245/',
   method: 'GET',
   charset: 'UTF-8',

   stableParse: '2010-03-18',

   result: {
      'ipv4': null,
      'ipv6': null,
   },

   parse: function(html) {

      this.result.ipv4 = html.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/gi)[0];
      return this.result;
   }

});

externIpServices.push({

      name: 'CGI script by Chris F.A. Johnson',

      url: 'http://cfaj.freeshell.org/ipaddr.cgi',
      method: 'GET',
      charset: 'UTF-8',

      stableParse: '2010-03-18',

      result: {
         'ipv4': null,
         'ipv6': null,
      },

      parse: function(html) {

         this.result.ipv4 = html;
         return this.result;
      }
});

