/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/* External info object */
Conetserv.ExternalInfo = {
   isRunning : 0,

   start : function ()  {
      if(this.isRunning)
         return;
      this.isRunning = 1;

      Conetserv.Map.setElementId("map-placeholder");

      /* Start external info services */
      Services.start(
         /* started */
         function() {
            Ui.addIcons(".external", ".provider", "");
         },
         /* service results */
         function(service, result) {
            var source = ' <span class="serviceSource">(<a href="' + service.link + '" target="_blank">' + service.name + '</a>)</span>';
            if (result.externIpv4)
               $("#externIpv4").append('<li class="ui-corner-all"><strong>' + result.externIpv4 + '</strong> ' + source + '</li>');
            if (result.externIpv6)
               $("#externIpv6").append('<li class="ui-corner-all"><strong>' + result.externIpv6 + '</strong> ' + source + '</li>');
            if (result.provider)
               $("#provider").append('<li class="ui-corner-all"><strong>' + result.provider + '</strong> ' + source + '</li>');
            if (result.hostname)
               $("#hostname").append('<li class="ui-corner-all"><strong>' + result.hostname + '</strong> ' + source + '</li>');
            if (result.route)
               $("#route").append('<li class="ui-corner-all"><strong>' + result.route + '</strong> ' + source + '</li>');
            if (result.city || result.region || result.country ||
               result.countryCode || result.longitude || result.latitude) {
               $("#location").append(
                  '<li class="ui-corner-all"><strong>' +
                  (result.city ? result.city + ', ' : '') +
                  (result.region ? result.region + ', ' : '') +
                  (result.country ? result.country : '') +
                  (result.countryCode ? ' [' + result.countryCode + ']' : '') +
                  '</strong> ' +
                  (result.longitude ? 'Longitude: ' + result.longitude : '') +
                  (result.latitude ? ', Latitude: ' + result.latitude : '') +
                  source + '</li>'
                  );

               // show map location
               if(result.longitude && result.latitude) {
                  /* set element to write map into */
                  Conetserv.Map.addLocation(service, result);
                  //check, if page is defaultly shown, otherwise show on buttonclick
                  if($("#external-info input[type=radio]:checked").val() == "external-map-div") {
                     Conetserv.Map.show();
                  } else {
                     $("#external-map").click(function(){
                        Conetserv.Map.show();
                     });
                  }
               }
            }
         },
         /* stopped */
         function() {
            Ui.removeIcons(".external", ".provider");
         }
      );
   }
}