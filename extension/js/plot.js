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

var testdata1 = " 1  192.168.1.1 (192.168.1.1)  4.106 ms   5.869 ms   11.442 ms ";
var testdata2 =	" 2  78.184.32.1 (78.184.32.1)  200.409 ms   74.395 ms   69.322 ms";
var testdata3 =	" 3  * * *";
var testdata4 =	" 4  gayrettepe-t2-2-gayrettepe-t3-1.turktelekom.com.tr (212.156.118.5)  71.433 ms   70.908 ms   70.728 ms";
var testdata5 =	" 5  static.turktelekom.com.tr (212.156.103.33)  200.774 ms   200.347 ms   200.714 ms";

/* struct for returning parsed data */
function Item(id, dns, ip, timeSt, timeMid, timeEnd)
{
   this.id = id;
   this.dns = dns;
   this.ip = ip;
   this.timeSt = timeSt;
   this.timeMid = timeMid;
   this.timeEnd = timeEnd;
}


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

   var startPos = pingCount > 30? pingCount - 30 : 1;
   var endPos = pingCount > 10? pingCount : 10;
   /*update chart*/
   $.plot(pingPlaceholder, [{ data: pingData, label: "Latency [ms]", color: "#2779AA" }], $.extend(true, {}, options, {
            xaxis: { min: startPos, max: endPos}
         }));
}

/* draw plot */
function drawTraceroutePlot(newdata)
{
   /* prepare data */
   var tmp = newdata.replace(/\s+/g, ' ');	/* remove multiple spaces */
   var rows = tmp.split("\n");		/* divide into rows */

   for( i = 0; i < rows.length ; i++)		/* adding row data to structures for plot */
   {
      var fields = rows[i].split(" ");
      var step, time, label;

      if($.client.os == "Windows")
      {
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
