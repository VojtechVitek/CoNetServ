/* reading loop intervals */
var pingInterval = -1;
var ping6Interval = -1;
var tracerouteInterval = -1;
var traceroute6Interval = -1;
var whoisInterval = -1;
 
/* CoNetServ NPAPI plugin to interact native code with */
var conetserv = document.getElementById("conetserv");
 
/* console text-boxes */
var pingConsole = document.getElementById("pingConsole");
var ping6Console = document.getElementById("ping6Console");
var tracerouteConsole = document.getElementById("tracerouteConsole");
var traceroute6Console = document.getElementById("traceroute6Console");
var whoisConsole = document.getElementById("whoisConsole");
 
function readPing()
{
   var received;
   try {
      received = document.getElementById("conetserv").readPing();
   }
   catch(e) {
      document.getElementById("pingConsole").value = e;
      return;
   }
   if (received === false) {
      stopAnim("pingState");
      window.clearInterval(pingInterval);
      pingInterval = -1;
      return;
   }
   pingConsole.value = document.getElementById("pingConsole").value + received;
   plotPing(received, 4);
}
 
function readPing6()
{
   var received;
   try {
      received = document.getElementById("conetserv").readPing6();
   }
   catch(e) {
      document.getElementById("ping6Console").value = e;
      return;
   }
   if (received === false) {
      stopAnim("ping6State");
      window.clearInterval(ping6Interval);
      ping6Interval = -1;
      return;
   }
   ping6Console.value = document.getElementById("ping6Console").value + received;
   plotPing(received, 6);
}
 
function readTraceroute()
{
   var received;
   try {
      received = document.getElementById("conetserv").readTraceroute();
   }
   catch(e) {
      document.getElementById("tracerouteConsole").value = e;
   }
   if (received === false) {
      stopAnim("tracerouteState");
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = -1;
      return;
   }
   document.getElementById("tracerouteConsole").value = document.getElementById("tracerouteConsole").value + received;
   plotTraceroute(received, 4);
}
 
function readTraceroute6()
{
   var received;
   try {
      received = document.getElementById("conetserv").readTraceroute6();
   }
   catch(e) {
      document.getElementById("traceroute6Console").value = e;
   }
   if (received === false) {
      stopAnim("traceroute6State");
      window.clearInterval(traceroute6Interval);
      traceroute6Interval = -1;
      return;
   }
   document.getElementById("traceroute6Console").value = document.getElementById("traceroute6Console").value + received;
   plotTraceroute(received, 6);
}
 
function readWhois()
{
   var received;
   try {
      received = document.getElementById("conetserv").readWhois();
   }
   catch(e) {
      document.getElementById("whoisConsole").value = e;
   }
   if (received === false) {
      stopAnim("whoisState");
      window.clearInterval(whoisInterval);
      whoisInterval = -1;
      return;
   }
   document.getElementById("whoisConsole").value = document.getElementById("whoisConsole").value + received;
}
 
function startCommands()
{
   if (pingInterval == -1) {
      try {
         document.getElementById("pingConsole").value = "";
         if (document.getElementById("conetserv").startPing(document.getElementById("url").value)) {
             /* reset data and start animation */
             pingData = new Data();
             startAnim("pingState");
             pingInterval = window.setInterval("readPing()", 500);
             readPing();
         } else {
            pingInterval = -1;
         }
      }
      catch(e) {
         document.getElementById("pingConsole").value = e;
         pingInterval = -1;
      }
   }
 
   if (ping6Interval == -1) {
      try {
         document.getElementById("ping6Console").value = "";
         if (document.getElementById("conetserv").startPing6(document.getElementById("url").value)) {
             /* reset data and start animation */
             ping6Data = new Data();
             startAnim("ping6State");
             ping6Interval = window.setInterval("readPing6()", 500);
             readPing6();
         } else {
            ping6Interval = -1;
         }
      }
      catch(e) {
         document.getElementById("ping6Console").value = e;
         ping6Interval = -1;
      }
   }
 
   if (tracerouteInterval == -1) {
      try {
         document.getElementById("tracerouteConsole").value = "";
         if (document.getElementById("conetserv").startTraceroute(document.getElementById("url").value)) {
            traceData = new Data();
            startAnim("tracerouteState");
            tracerouteInterval = window.setInterval("readTraceroute()", 500);
            readTraceroute();            
         }
         else {
            tracerouteInterval = -1;
         }
      }
      catch(e) {
         document.getElementById("tracerouteConsole").value = e;
         tracerouteInterval = -1;
      }
   }
 
   if (traceroute6Interval == -1) {
      try {
         document.getElementById("traceroute6Console").value = "";
         if (document.getElementById("conetserv").startTraceroute6(document.getElementById("url").value)) {
            trace6Data = new Data();
            startAnim("traceroute6State");
            traceroute6Interval = window.setInterval("readTraceroute6()", 500);
            readTraceroute6();
         }
         else {
            traceroute6Interval = -1;
         }
      }
      catch(e) {
         document.getElementById("traceroute6Console").value = e;
         traceroute6Interval = -1;
      }
   }
 
   if (whoisInterval == -1) {
      try {
         document.getElementById("whoisConsole").value = "";
         if (document.getElementById("conetserv").startWhois(document.getElementById("url").value)) {
            whoisInterval = window.setInterval("readWhois()", 500);
            readWhois();
            startAnim("whoisState");
         }
         else {
            whoisInterval = -1;
         }
      }
      catch(e) {
         document.getElementById("whoisConsole").value = e;
         whoisInterval = -1;
      }
   }
}
 
function stopCommands()
{
   stopPing();
   stopPing6();
   stopTraceroute();
   stopTraceroute6();
   stopWhois();
}
 
function stopPing()
{
   if (pingInterval != -1) {
      try {
         document.getElementById("conetserv").stopPing();
      }
      catch(e) {
         document.getElementById("pingConsole").value = document.getElementById("pingConsole").value + e;
      }
      stopAnim("pingState");
      window.clearInterval(pingInterval);
      pingInterval = -1;
   }
}
 
function stopPing6()
{
   if (ping6Interval != -1) {
      try {
         document.getElementById("conetserv").stopPing6();
      }
      catch(e) {
         document.getElementById("ping6Console").value = document.getElementById("ping6Console").value + e;
      }
      stopAnim("ping6State");
      window.clearInterval(ping6Interval);
      ping6Interval = -1;
   }
}
 
function stopTraceroute()
{
   if (tracerouteInterval != -1) {
      try {
         document.getElementById("conetserv").stopTraceroute();
      }
      catch(e) {
         document.getElementById("tracerouteConsole").value = document.getElementById("tracerouteConsole").value + e;
      }
      stopAnim("tracerouteState");
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = -1;
   }
}
 
function stopTraceroute6()
{
   if (traceroute6Interval != -1) {
      try {
         document.getElementById("conetserv").stopTraceroute6();
      }
      catch(e) {
         document.getElementById("traceroute6Console").value = document.getElementById("traceroute6Console").value + e;
      }
      stopAnim("traceroute6State");
      window.clearInterval(traceroute6Interval);
      traceroute6Interval = -1;
   }
}
 
function stopWhois()
{
   if (whoisInterval != -1) {
      try {
         document.getElementById("conetserv").stopWhois();
      }
      catch(e) {
         document.getElementById("whoisConsole").value = document.getElementById("whoisConsole").value + e;
      }
      stopAnim("whoisState");
      window.clearInterval(whoisInterval);
      whoisInterval = -1;
   }
}