
var pingPlaceholder = $("#pingPlaceholder");
var ping6Placeholder = $("#ping6Placeholder");
var traceroutePlaceholder = $("#traceroutePlaceholder");
var traceroute6Placeholder = $("#traceroute6Placeholder");

/* ping time data */
function Data() {
  this.rows = [];
  this.prevId = 0;
  this.count = 0;
}

var pingData = new Data();
var ping6Data = new Data();


var traceData = [];
var traceLabels = [];

var tracePrevData = "";


/* chart data */
var optionsPing = {
      lines: { show: true },
      legend: { show: true, position: "sw", backgroundOpacity: 0.5 },
      points: { show: true },
      xaxis: { tickDecimals: 0, tickSize: 1 },
      yaxis: { min: 0}
};

var optionsTrace = {
      lines: { show: true },
      legend: { show: true, position: "sw", backgroundOpacity: 0.5 },
      points: { show: true },
      xaxis: { tickDecimals: 0, tickSize: 1, min: 0, max: (traceData.length + 2) > 10? (traceData.length + 2) : 10, zoomRange: [0.1, 10], panRange: [0, 30] },
      zoom: { interactive: false },
      pan: { interactive: false },
      valueLabels: { show: true },
      legend: { position: "nw" }
};

/* bind plot functions to tab changing */
$('#tabs').bind('tabsshow', function() {
   var $tabs = $('#tabs').tabs();
   var selected = $tabs.tabs('option', 'selected');
   if(selected == "0")
      $.plot(pingPlaceholder, [{ data: pingData.rows, label: "Latency [ms]", color: "#2779AA" }], $.extend(true, {}, optionsPing, {
      xaxis: { min: 0, max: pingData.count > 10? pingData.count + 1 : 10}
      }));
   if(selected == "2")
      $.plot(traceroutePlaceholder, [{ data: traceData, label: "Position", color: "#2779AA" }], $.extend(true, {}, optionsTrace, {
      }));
});

function plotPing(received, type)
{
   var pingTime;
   var data = type == 4? pingData : ping6Data;
   
   /* update chart data */
   
   if($.client.os == "Windows") {
      if(received.indexOf("Odpoved") != -1) {
         data.count++;
         pingTime= parseInt(received.substr(received.indexOf("cas=")+4,received.indexOf("ms")-received.indexOf("cas=")+4));
         data.rows.push([pingData.count, pingTime]);
      } else if(received.indexOf("Reply") != -1) {
         data.count++;
         pingTime= parseInt(received.substr(received.indexOf("time=")+5));
         data.rows.push([pingData.count, pingTime]);
      } else if(received.indexOf("Vyprsel") != -1 || received.indexOf("neni dostupny") != -1 || received.indexOf("timed out") != -1) {
         data.count++;
         data.rows.push(null);
      }
   } else if($.client.os == "Linux") {
      if(received.indexOf("bytes from") != -1) {
         /* create "holes" in plot in case of lost packet */
         var actPingId = parseInt(received.substr(received.indexOf("icmp_seq=")+9));
         /* check id */
         while(actPingId > ++data.prevId) {
            data.count++;
            data.rows.push(null);
         }
         data.count++;
         pingTime= parseInt(received.substr(received.indexOf("time=")+5,received.indexOf(" ms")-received.indexOf("time=")+5));
         data.rows.push([data.count, pingTime]);
         
      }
   } 
      
   if(data.rows.length > 30)
      data.rows.shift();   

   /*update chart*/
   if(type == 4)
      $.plot(pingPlaceholder, [{ data: pingData.rows, label: "Latency [ms]", color: "#2779AA" }], $.extend(true, {}, optionsPing, {
         xaxis: { min: 0, max: pingData.count > 10? pingData.count + 1 : 10}
         }));
   else
      $.plot(ping6Placeholder, [{ data: ping6Data.rows, label: "Latency [ms]", color: "#2779AA" }], $.extend(true, {}, optionsPing, {
         xaxis: { min: 0, max: ping6Data.count > 10? ping6Data.count + 1 : 10}
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
function plotTraceroute(received, type)
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

   $.plot(traceroutePlaceholder, [{ data: traceData, label: "Position", color: "#2779AA" }], $.extend(true, {}, optionsTrace, {
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
