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

/* reading loop intervals */
var pingInterval = false;
var tracerouteInterval = false;
var whoisInterval = false;

/* previous ping packet id */
var prevPingId = 0;

/* chart data */
var options = {
      lines: { show: true },
      points: { show: true },
      xaxis: { tickDecimals: 0, tickSize: 1 },
      yaxis: { min: 0}    
};
var placeholder = $("#placeholder");
/* ping time data */
var pingData = [];
var pingCount = 0;

$(function () {
   $.plot(placeholder, [{ data: pingData, label: "Latency [ms]", color: "#2779AA" }], options);
});

function startAnim(id)
{
   document.getElementById(id).style.visibility = 'visible';
}

function stopAnim(id)
{
   document.getElementById(id).style.visibility = 'hidden';
}

function readPing() {   
   try {
      var received = document.getElementById("conetserv").readPing();
      pingConsole.value = document.getElementById("pingConsole").value + received;
      /*update chart data*/
      if($.client.os == "Windows")
      {
	 if(received.indexOf("Odpoved") != -1)
	 {
	    pingCount++;
	    pingTime= parseInt(received.substr(received.indexOf("cas=")+4,received.indexOf("ms")-received.indexOf("cas=")+4));
	    pingData.push([pingCount, pingTime]);
	 }
	 else if(received.indexOf("Reply") != -1)
	 {
	    pingCount++;
	    pingTime= parseInt(received.substr(received.indexOf("time=")+5));
	    pingData.push([pingCount, pingTime]);
	    
	 }
	 else if(received.indexOf("Vyprsel") != -1 || received.indexOf("neni dostupny") != -1 || received.indexOf("timed out") != -1)
	 {
	    pingCount++;
	    pingData.push(null);
	 }
      }

      else if($.client.os == "Linux")
      {
	 if(received.indexOf("bytes from") != -1)
	 {
	    /* create "holes" in plot in case of lost packet */
	    var actPingId = parseInt(received.substr(received.indexOf("icmp_seq=")+9));
	    /* check id */
	    while(actPingId > ++prevPingId)
	    {
	       pingCount++;
	       pingData.push(null);
	    }
	    pingCount++;
	    pingTime= parseInt(received.substr(received.indexOf("time=")+5,received.indexOf(" ms")-received.indexOf("time=")+5));
	    pingData.push([pingCount, pingTime]);
	 }
     }
     else pingConsole.value += $.client.os;

     var startPos = pingCount > 30? pingCount - 30 : 1;
     var endPos = pingCount > 10? pingCount : 10;
     /*update chart*/
     $.plot(placeholder, [{ data: pingData, label: "Latency [ms]", color: "#2779AA" }], $.extend(true, {}, options, {
                              xaxis: { min: startPos, max: endPos}
                          }));
   }
   catch(e) {
      document.getElementById("pingConsole").value = e;
   }
}

function readTraceroute() {
   try {
      document.getElementById("tracerouteConsole").value = document.getElementById("tracerouteConsole").value + document.getElementById("conetserv").readTraceroute();
   }
   catch(e) {
      document.getElementById("tracerouteConsole").value = e;
   }
}

function readWhois() {
   try {
      document.getElementById("whoisConsole").value = document.getElementById("whoisConsole").value + document.getElementById("conetserv").readWhois();
   }
   catch(e) {
      document.getElementById("whoisConsole").value = e;
   }
}

function startCommands() {
   if (pingInterval == -1) {
      try {
         document.getElementById("pingConsole").value = "";
         if (document.getElementById("conetserv").startPing(document.getElementById("url").value))
	 {
	    /* reset data and start animation */
	    pingData = [];
	    pingCount = 0;
	    prevPingId = 0;
	    startAnim("pingState");

            pingInterval = window.setInterval("readPing()", 500);
	 }
         else
            pingInterval = -1;
      }
      catch(e) {
         document.getElementById("pingConsole").value = e;
         pingInterval = -1;
      }
   }

   if (tracerouteInterval == -1) {
      try {
         document.getElementById("tracerouteConsole").value = "";
         if (document.getElementById("conetserv").startTraceroute(document.getElementById("url").value))
            tracerouteInterval = window.setInterval("readTraceroute()", 500);
         else
            tracerouteInterval = -1;
      }
      catch(e) {
         document.getElementById("tracerouteConsole").value = e;
         tracerouteInterval = -1;
      }
   }

   if (whoisInterval == -1) {
      try {
         document.getElementById("whoisConsole").value = "";
         if (document.getElementById("conetserv").startWhois(document.getElementById("url").value))
            whoisInterval = window.setInterval("readWhois()", 500);
         else
            whoisInterval = -1;
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
