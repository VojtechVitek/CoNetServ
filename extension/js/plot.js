/* previous ping packet id */
var prevPingId = 0;

/* chart data */
var options = {
      lines: { show: true },
      legend: { show: true, position: "sw", backgroundOpacity: 0.5 },
      points: { show: true },
      xaxis: { tickDecimals: 0, tickSize: 1 },
      yaxis: { min: 0}
};

$(function(){
   $('#tabs').tabs({
      show: function(event, ui) {
          $.plot(pingPlaceholder, [{ data: pingData, label: "Latency [ms]", color: "#2779AA" }], options);
              return false;
         
      }
   });
});



var pingPlaceholder = $("#pingPlaceholder");
var traceroutePlaceholder = $("#traceroutePlaceholder");

$(function () {
   $.plot(pingPlaceholder, [{ data: pingData, label: "Latency [ms]", color: "#2779AA" }], options);
});

/* ping time data */
var pingData = [];
var pingCount = 0;

var traceData = [];
var traceLabels = [];

var tracePrevData = "";

function plotPing(received)
{
   /* update chart data */
   if($.client.os == "Windows") {
      if(received.indexOf("Odpoved") != -1) {
         pingCount++;
         pingTime= parseInt(received.substr(received.indexOf("cas=")+4,received.indexOf("ms")-received.indexOf("cas=")+4));
         pingData.push([pingCount, pingTime]);
      } else if(received.indexOf("Reply") != -1) {
         pingCount++;
         pingTime= parseInt(received.substr(received.indexOf("time=")+5));
         pingData.push([pingCount, pingTime]);
      } else if(received.indexOf("Vyprsel") != -1 || received.indexOf("neni dostupny") != -1 || received.indexOf("timed out") != -1) {
         pingCount++;
         pingData.push(null);
      }
   } else if($.client.os == "Linux") {
      if(received.indexOf("bytes from") != -1) {
         /* create "holes" in plot in case of lost packet */
         var actPingId = parseInt(received.substr(received.indexOf("icmp_seq=")+9));
         /* check id */
         while(actPingId > ++prevPingId) {
            pingCount++;
            pingData.push(null);
         }
         pingCount++;
         pingTime= parseInt(received.substr(received.indexOf("time=")+5,received.indexOf(" ms")-received.indexOf("time=")+5));
         pingData.push([pingCount, pingTime]);
         
      }
   } else {
      pingConsole.value += $.client.os;
   }
   
   
   if(pingData.length > 30)
      pingData.shift();   

   /*update chart*/
   $.plot(pingPlaceholder, [{ data: pingData, label: "Latency [ms]", color: "#2779AA" }], $.extend(true, {}, options, {
            
         }));
}

function addTPlotDataWin(row)
{
   var nospaces = row.replace(/\s+/g, ' ');	/* remove multiple spaces */
   var fields = nospaces.split(" ");
   var step, time, label, first = 0, labelPos = 4;
   
   while(fields[first] == "" && first<fields.length-6)
      first++;
   
   if(!(step = parseInt(fields[first])))
      return;
      
   for( j = first; j < fields.length; j++)
      if(fields[j] == "ms") 
         labelPos++;
         
   time = (time = parseFloat(fields[first+1]))? time : (time = parseFloat(fields[first+2])) ? time : parseFloat(fields[first+3]);
   label = fields[first + labelPos];
   
   traceData.push([step, time]);
   traceLabels.push(label);
}

/* draw plot */
function plotTraceroute(received)
{
   if(received == "")
      return;
   /* connect with previous data */
   tracePrevData += received;
   /* prepare data */
   
   if($.client.os == "Windows")
   {
      var npos;
      /* divide input data into lines and add them as data */
      while((npos = tracePrevData.indexOf("\n")) != -1)
      {
         addTPlotDataWin(tracePrevData.substr(0, npos));
         tracePrevData = (tracePrevData.substr(npos+1));
      }
   }   
   else if($.client.os == "Linux")
   {
      step = parseFloat(fields[1]);
      /* check for not comming packets */
      if(fields[2] == "*")
      {
       if(fields[3] != "*")
       {
          label = fields[3];
          time = parseInt(fields[5]);
          traceData.push([step, time]);
          traceLabels.push(label);
       }
      }
      else
      {
          label = fields[2];
          time = parseInt(fields[4]);
          //tracerouteConsole.value += fields[3];
          traceData.push([step, time]);
          traceLabels.push(label);
      }

   }
   else
   {
   }

   $.plot(traceroutePlaceholder, [{ data: traceData, label: "Position", color: "#2779AA" }], $.extend(true, {}, options, {
            xaxis: { autoscaleMargin: 1 },
            valueLabels: { show: true },
            legend: { position: "nw" }
            }));
}

function startAnim(id)
{
   document.getElementById(id).style.visibility = 'visible';
}

function stopAnim(id)
{
   document.getElementById(id).style.visibility = 'hidden';
}
