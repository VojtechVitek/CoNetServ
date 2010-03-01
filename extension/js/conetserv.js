/* reading loop intervals */
var pingInterval = -1;
var tracerouteInterval = -1;
var whoisInterval = -1;

/* CoNetServ NPAPI plugin to interact native code with */
var conetserv = document.getElementById("conetserv");

/* url to apply network tools for */
var conetservUrl = document.getElementById("conetservUrl");

/* console text-boxes */
var pingConsole = document.getElementById("pingConsole");
var tracerouteConsole = document.getElementById("tracerouteConsole");
var whoisConsole = document.getElementById("whoisConsole");

function readPing() {
   var received;
   try {
      received = document.getElementById("conetserv").readPing();
   }
   catch(e) {
      document.getElementById("pingConsole").value = e;
      return;
   }
   if (received === false) {
      window.clearInterval(pingInterval);
      pingInterval = -1;
      return;
   }
   pingConsole.value = document.getElementById("pingConsole").value + received;
   plotPing(received);
}

function readTraceroute() {
   var received;
   try {
      received = document.getElementById("conetserv").readTraceroute();
   }
   catch(e) {
      document.getElementById("tracerouteConsole").value = e;
   }
   if (received === false) {
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = -1;
      return;
   }
   document.getElementById("tracerouteConsole").value = document.getElementById("tracerouteConsole").value + received;
   plotTraceroute(received);
}

function readWhois() {
   var received;
   try {
      received = document.getElementById("conetserv").readWhois();
   }
   catch(e) {
      document.getElementById("whoisConsole").value = e;
   }
   if (received === false) {
      window.clearInterval(whoisInterval);
      whoisInterval = -1;
      return;
   }
   document.getElementById("whoisConsole").value = document.getElementById("whoisConsole").value + received;
}

function startCommands() {

   if (pingInterval == -1) {
      try {
         document.getElementById("pingConsole").value = "";
         if (document.getElementById("conetserv").startPing(document.getElementById("url").value)) {
             /* reset data and start animation */
             pingData = [];
             pingCount = 0;
             prevPingId = 0;
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

   if (tracerouteInterval == -1) {
      try {
         document.getElementById("tracerouteConsole").value = "";
         if (document.getElementById("conetserv").startTraceroute(document.getElementById("url").value)) {
            tracerouteInterval = window.setInterval("readTraceroute()", 500);
            readTraceroute();
            traceData = [];
            traceLabels = [];
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

   if (whoisInterval == -1) {
      try {
         document.getElementById("whoisConsole").value = "";
         if (document.getElementById("conetserv").startWhois(document.getElementById("url").value)) {
            whoisInterval = window.setInterval("readWhois()", 500);
            readWhois();
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

function stopCommands() {
   if (pingInterval != -1) {
      try {
         document.getElementById("conetserv").stopPing();
      }
      catch(e) {
         document.getElementById("pingConsole").value = document.getElementById("pingConsole").value + e;
      }
      window.clearInterval(pingInterval);
      pingInterval = -1;
   }

   if (tracerouteInterval != -1) {
      try {
         document.getElementById("conetserv").stopTraceroute();
      }
      catch(e) {
         document.getElementById("tracerouteConsole").value = document.getElementById("tracerouteConsole").value + e;
      }
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = -1;
   }

   if (whoisInterval != -1) {
      try {
         document.getElementById("conetserv").stopWhois();
      }
      catch(e) {
         document.getElementById("whoisConsole").value = document.getElementById("whoisConsole").value + e;
      }
      window.clearInterval(whoisInterval);
      whoisInterval = -1;
   }
}
