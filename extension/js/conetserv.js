/* reading loop intervals */
var pingInterval = -1;
var ping6Interval = -1;
var tracerouteInterval = -1;
var traceroute6Interval = -1;
var whoisInterval = -1;
var nslookupInterval = -1;

/* CoNetServ NPAPI plugin to interact native code with */
var conetserv;

/* console text-boxes */
var pingConsole;
var ping6Console;
var tracerouteConsole;
var traceroute6Console;
var whoisConsole;
var nslookupConsole;

/* local info running */
var isLocalInfoRunning = 0;

function startAnim(id)
{
   document.getElementById(id+"State").style.display = 'block';//visibility = 'visible';
   document.getElementById(id+"TabClose").style.display = 'block';
}

function stopAnim(id)
{
   document.getElementById(id+"State").style.display = 'none';//visibility = 'hidden';
   document.getElementById(id+"TabClose").style.display = 'none';
}

$(document).ready(function(){

   /* Toggle tabs with opacity effect write toggle instead of none to enable*/
   //$("#tabs").tabs({ fx: { opacity: 'none' } });

   /* CoNetServ object - NPAPI plugin */
   conetserv = document.getElementById("conetserv");

   /* console text-boxes */
   pingConsole = new console("pingConsole");
   pingConsole.maxRows = 16;
   ping6Console = new console("ping6Console");
   ping6Console.maxRows = 16;
   tracerouteConsole = new console("tracerouteConsole");
   traceroute6Console = new console("traceroute6Console");
   whoisConsole = new console("whoisConsole");
   nslookupConsole = new console("nslookupConsole");

   /* init url in Firefox */
   if ($.client.browser == "Firefox") {
      if (window && window.arguments && window.arguments[0] && url.set(window.arguments[0])) {
         document.getElementById("url").value = url.hostname;
      }
   /* init url in Chrome */
   } else if ($.client.browser == "Chrome") {
      if (chrome && chrome.tabs && chrome.tabs.getSelected) {
         chrome.tabs.getSelected(null, function(tab) {
            url.set(tab.url);
            document.getElementById("url").value = url.hostname;
         });
      }
   }
});

function startLocalInfo()
{
   if(isLocalInfoRunning)
      return;
   
   isLocalInfoRunning = 1;

   /* Start local info services */   
   Services.start(
      /* started */
      function() {
         startAnim("local");
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
               (result.region ? + result.region + ', ' : '') +
               (result.country ? result.country : '') +
               (result.countryCode ? ' [' + result.countryCode + ']' : '') +
               '</strong> ' + 
               (result.longitude ? 'Longitude: ' + result.longitude : '') +
               (result.latitude ? ', Latitude: ' + result.latitude : '') +
               source + '</li>'
            );
            //show map
            if(result.longitude) {
               Map.inicialize(result.longitude, result.latitude);
               Map.show();
            }
         }
      },
      /* stopped */
      function() {
         stopAnim("local");
      }
   );
};

/** 
 * Checks address for validity to ping, traceroute,...
 *
 */
function checkAddress(addr)
{
   var IPv4regxp = /^(f|ht)tp[s]{0,1}:[/]{2}\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/ig;
   var http = /(http|https|ftp)([^ ]+)/ig
   return IPv4regxp.exec(addr) || http.exec(addr);
}

/**
 * Read output of PING6 command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readPing()
{
   var received;
   try {
      received = document.getElementById("conetserv").readPing();
   }
   catch(e) {
      pingConsole.add(e);
      return;
   }
   if (received === false) {
      stopAnim("ping");
      window.clearInterval(pingInterval);
      pingInterval = -1;
      return;
   }
   pingConsole.add(received);
   plotPing(received, 4);
}

/**
 * Read output of PING6 command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readPing6()
{
   var received;
   try {
      received = document.getElementById("conetserv").readPing6();
   }
   catch(e) {
      ping6Console.add(e);
      return;
   }
   if (received === false) {
      stopAnim("ping6");
      window.clearInterval(ping6Interval);
      ping6Interval = -1;
      return;
   }
   ping6Console.add(received);
   plotPing(received, 6);
}

/**
 * Read output of TRACEROUTE command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readTraceroute()
{
   var received;
   try {
      received = document.getElementById("conetserv").readTraceroute();
   }
   catch(e) {
      tracerouteConsole.add(e);
   }
   if (received === false) {
      stopAnim("traceroute");
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = -1;
      return;
   }
   tracerouteConsole.add(received);
   plotTraceroute(received, 4);
}

/**
 * Read output of TRACEROUTE6 command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readTraceroute6()
{
   var received;
   try {
      received = document.getElementById("conetserv").readTraceroute6();
   }
   catch(e) {
      traceroute6Console.add(e);
   }
   if (received === false) {
      stopAnim("traceroute6");
      window.clearInterval(traceroute6Interval);
      traceroute6Interval = -1;
      return;
   }
   traceroute6Console.add(received);
   plotTraceroute(received, 6);
}

/**
 * Read output of WHOIS command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readWhois()
{
   var received;
   try {
      received = document.getElementById("conetserv").readWhois();
   }
   catch(e) {
      whoisConsole.add(e);
   }
   if (received === false) {
      stopAnim("whois");
      window.clearInterval(whoisInterval);
      whoisInterval = -1;
      return;
   }
   whoisConsole.add(received);
}

/**
 * Read output of NSLOOKUP command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readNslookup()
{
   var received;
   try {
      received = document.getElementById("conetserv").readNslookup();
   }
   catch(e) {
      nslookupConsole.add(e);
   }
   if (received === false) {
      stopAnim("nslookup");
      window.clearInterval(nslookupInterval);
      nslookupInterval = -1;
      return;
   }
   nslookupConsole.add(received);
}

/**
 * Start all commands available at once.
 */
function startCommands()
{
   //set value for commands
   if(!url.set(document.getElementById("url").value)) {
      document.getElementById("url").style.color="red";
      document.getElementById("url").focus();
      return;
   }

   startPing();
   startPing6();
   startTraceroute();
   startTraceroute6();
   startNslookup();
   if($.client.os != "Windows")
      startWhois();
}

/**
 * Start PING command
 */
function startPing()
{
   if (pingInterval == -1) {
      try {
         pingConsole.clear();
         if (document.getElementById("conetserv").startPing(url.hostname)) {
             /* reset data and start animation */
             pingData.reset(); 
             startAnim("ping");
             pingInterval = window.setInterval("readPing()", 500);
             readPing();
         } else {
            pingInterval = -1;
         }
      }
      catch(e) {
         pingConsole.add(e);
         pingInterval = -1;
      }
   }
}

/**
 * Start PING6 command
 */
function startPing6()
{
   if (ping6Interval == -1) {
      try {
         ping6Console.clear();
         if (document.getElementById("conetserv").startPing6(url.hostname)) {
             /* reset data and start animation */
             ping6Data.reset();
             startAnim("ping6");
             ping6Interval = window.setInterval("readPing6()", 500);
             readPing6();
         } else {
            ping6Interval = -1;
         }
      }
      catch(e) {
         ping6Console.add(e);
         ping6Interval = -1;
      }
   }
}

/**
 * Start TRACEROUTE command
 */
function startTraceroute()
{
   if (tracerouteInterval == -1) {
      try {
         tracerouteConsole.clear();
         if (document.getElementById("conetserv").startTraceroute(url.hostname)) {
            traceData.reset();
            startAnim("traceroute");
            tracerouteInterval = window.setInterval("readTraceroute()", 500);
            readTraceroute();            
         }
         else {
            tracerouteInterval = -1;
         }
      }
      catch(e) {
         tracerouteConsole.add(e);
         tracerouteInterval = -1;
      }
   }
}

/**
 * Start TRACEROUTE6 command
 */
function startTraceroute6()
{
   if (traceroute6Interval == -1) {
      try {
         traceroute6Console.clear();
         if (document.getElementById("conetserv").startTraceroute6(url.hostname)) {
            trace6Data.reset();
            startAnim("traceroute6");
            traceroute6Interval = window.setInterval("readTraceroute6()", 500);
            readTraceroute6();
         }
         else {
            traceroute6Interval = -1;
         }
      }
      catch(e) {
         traceroute6Console.add(e);
         traceroute6Interval = -1;
      }
   }
}

/**
 * Start WHOIS command
 */
function startWhois()
{
   if (whoisInterval == -1) {
      try {
         whoisConsole.clear();
         if (document.getElementById("conetserv").startWhois(url.hostname)) {
            whoisInterval = window.setInterval("readWhois()", 500);
            startAnim("whois");
	    readWhois();
            
         }
         else {
            whoisInterval = -1;
         }
      }
      catch(e) {
         whoisConsole.add(e);
         whoisInterval = -1;
      }
   }
}

/**
 * Start NSLOOKUP command
 */
function startNslookup()
{
   if (nslookupInterval == -1) {
      try {
         nslookupConsole.clear();
         if (document.getElementById("conetserv").startNslookup(url.hostname)) {
            nslookupInterval = window.setInterval("readNslookup()", 500);
            startAnim("nslookup");
	    readNslookup();
            
         }
         else {
            nslookupInterval = -1;
         }
      }
      catch(e) {
         nslookupConsole.add(e);
         nslookupInterval = -1;
      }
   }
}

/**
 * Stop all commands available at once.
 */
function stopCommands()
{
   stopPing();
   stopPing6();
   stopTraceroute();
   stopTraceroute6();
   stopNslookup();
   if($.client.os != "Windows") 
      stopWhois();
}

/**
 * Stop PING command
 */
function stopPing()
{
   if (pingInterval != -1) {
      try {
         document.getElementById("conetserv").stopPing();
      }
      catch(e) {
         pingConsole.add(e);
      }
      stopAnim("ping");
      window.clearInterval(pingInterval);
      pingInterval = -1;
   }
}

/**
 * Stop PING6 command
 */
function stopPing6()
{
   if (ping6Interval != -1) {
      try {
         document.getElementById("conetserv").stopPing6();
      }
      catch(e) {
         ping6Console.add(e);
      }
      stopAnim("ping6");
      window.clearInterval(ping6Interval);
      ping6Interval = -1;
   }
}

/**
 * Stop TRACEROUTE command
 */
function stopTraceroute()
{
   if (tracerouteInterval != -1) {
      try {
         document.getElementById("conetserv").stopTraceroute();
      }
      catch(e) {
         tracerouteConsole.add(e);
      }
      stopAnim("traceroute");
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = -1;
   }
}
 
/**
 * Stop TRACEROUTE6 command
 */
function stopTraceroute6()
{
   if (traceroute6Interval != -1) {
      try {
         document.getElementById("conetserv").stopTraceroute6();
      }
      catch(e) {
         traceroute6Console.add(e);
      }
      stopAnim("traceroute6");
      window.clearInterval(traceroute6Interval);
      traceroute6Interval = -1;
   }
}

/**
 * Stop WHOIS command
 */
function stopWhois()
{
   if (whoisInterval != -1) {
      try {
         document.getElementById("conetserv").stopWhois();
      }
      catch(e) {
         whoisConsole.add(e);
      }
      stopAnim("whois");
      window.clearInterval(whoisInterval);
      whoisInterval = -1;
   }
}

/**
 * Stop NSLOOKUP command
 */
function stopNslookup()
{
   if (nslookupInterval != -1) {
      try {
         document.getElementById("conetserv").stopNslookup();
      }
      catch(e) {
         nslookupConsole.add(e);
      }
      stopAnim("nslookup");
      window.clearInterval(nslookupInterval);
      nslookupInterval = -1;
   }
}

/**
 * Bind stopbuttons
 */
$('#pingTabClose').bind('onclick', function() {
   stopPing();
});
