//alert(document.getElementById('conetservDialog').defaultButton);
//alert(document.documentElement.getButton("accept").oncommand);

/* CoNetServ NPAPI plugin to interact native code with */
var conetserv = document.getElementById("conetserv");

/* url to apply network tools for */
var conetservUrl = document.getElementById("conetservUrl");

/* console text-boxes */
var pingConsole = document.getElementById("pingConsole");
var tracerouteConsole = document.getElementById("tracerouteConsole");
var whoisConsole = document.getElementById("whoisConsole");

/* reading loop intervals */
var pingInterval = false;
var tracerouteInterval = false;
var whoisInterval = false;

function start() {
   document.getElementById('conetservStartButton').focus();

   if (!pingInterval) {
      try {
         pingConsole.value = "";
         if (conetserv.startPing(conetservUrl.value))
            pingInterval = window.setInterval("readPing()", 500);
         else
            pingInterval = false;
      }
      catch(e) {
         pingConsole.value = e;
         pingInterval = false;
      }
   }

   if (!tracerouteInterval) {
      try {
         tracerouteConsole.value = "";
         if (conetserv.startTraceroute(conetservUrl.value))
            tracerouteInterval = window.setInterval("readTraceroute()", 500);
         else
            tracerouteInterval = false;
      }
      catch(e) {
         tracerouteConsole.value = e;
         tracerouteInterval = false;
      }
   }

   if (!whoisInterval) {
      try {
         whoisConsole.value = "";
         if (conetserv.startWhois(conetservUrl.value))
            whoisInterval = window.setInterval("readWhois()", 500);
         else
            whoisInterval = false;
      }
      catch(e) {
         whoisConsole.value = e;
         whoisInterval = false;
      }
   }
}

function stop()
{
   if (pingInterval) {
      try {
         conetserv.stopPing();
      }
      catch(e) {
         pingConsole.value = pingConsole.value + e;
      }
      window.clearInterval(pingInterval);
      pingInterval = false;
   }

   if (tracerouteInterval) {
      try {
         conetserv.stopTraceroute();
      }
      catch(e) {
         tracerouteConsole.value = tracerouteConsole.value + e;
      }
      window.clearInterval(tracerouteInterval);
      tracerouteInterval = false;
   }

   if (whoisInterval) {
      try {
         conetserv.stopWhois();
      }
      catch(e) {
         whoisConsole.value = whoisConsole.value + e;
      }
      window.clearInterval(whoisInterval);
      whoisInterval = false;
   }
}

function readPing() {
   try {
      pingConsole.value = pingConsole.value + conetserv.readPing();
   }
   catch(e) {
      pingConsole.value = e;
   }
}

function readTraceroute() {
   try {
      tracerouteConsole.value = tracerouteConsole.value + conetserv.readTraceroute();
   }
   catch(e) {
      tracerouteConsole.value = e;
   }
}

function readWhois() {
   try {
      whoisConsole.value = whoisConsole.value + conetserv.readWhois();
   }
   catch(e) {
      whoisConsole.value = e;
   }
}

function focusOnRun(){
  document.getElementById('conetservStartButton').focus();
}
