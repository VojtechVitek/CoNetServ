/* placeholders for plots */
var pingPlaceholder = $("#pingPlaceholder");
var ping6Placeholder = $("#ping6Placeholder");
var traceroutePlaceholder = $("#traceroutePlaceholder");
var traceroute6Placeholder = $("#traceroute6Placeholder");

/* variables for storing plot identifiers */
var traceroutePlot;
var traceroute6Plot;

/* variables for storing positions in traceroute plot */
var tracerouteAxes;
var traceroute6Axes;

/* Fix bug with fast changing of tabs */
var tabsLoading = 0;

/* ping time data object*/
function pData() {
   //data
   this.maxValues = 30;		//maximum number of shown values in plot
   this.rows = [];		//array for storing data
   this.count = 0;		//amount of data in array
   this.prevId = 0;		//id of previous ping packet

   this.sum = 0;		//sum of all values in fields
   this.minVal = 0;		//initiate min value
   this.maxVal = 0;		//initiate max value
   this.avrgVal = 0;		//initiate average value
   this.actVal = 0;		//actual value

   this.lost = 0;
   this.avrgs = [];		//array for storing average values
   this.min = [];
   this.max = [];
   
   this.prevData = "";  //data for storing received data from previous steps

   this.changed = 1;		//for checking for change in data since last tick
   //functions
   this.add = function (val) { 
      this.count++;
      this.changed = 1;
      
      if( val == null)
      {
      	 this.actVal = 0;
      	 this.rows.push(null);
      	 this.lost++;
      }
      else
      {
      	 this.actVal = parseFloat(val);
      	 this.rows.push([this.count, this.actVal]);
      	 this.sum += this.actVal;
         
      	 this.minVal = (this.minVal == 0 ? this.actVal : ( this.actVal < this.minVal ? this.actVal : this.minVal));
      	 this.maxVal = (this.maxVal == 0 ? this.actVal : ( this.actVal > this.maxVal ? this.actVal : this.maxVal));	 
      }   
      this.avrgVal = this.sum/this.count;
      
      this.min.push([this.count, this.minVal]);
      this.max.push([this.count, this.maxVal]);
      this.avrgs.push([this.count, this.avrgVal]);                
      //limit maximum data count in field
      if(this.rows.length > this.maxValues)
      {
	        this.rows.shift();  
      }
      if(this.avrgs.length > this.maxValues)
      {
      	 this.avrgs.shift();
      	 this.min.shift();  
      	 this.max.shift();  
      }
   };
   /* returns percentage of lost packets */
   this.getLostPercent = function (){
      var perc = this.lost > 0 ? this.lost/this.count*100 : 0;
      return perc.toFixed(2);
   };
}

/* traceroute data object */
function tData() {
   this.rows = [];		//array for storing data
   this.count = 0;		//amount of data in array

   this.prevId = 0;   
   this.prevData = "";
   this.labels = [];
   this.changed = 1;		//for checking for change in data since last tick
   //functions
   this.add = function (val, label) {
      this.count++;

      this.changed = 1;
      this.rows.push( [ this.count, (val == null || val == NaN) ? this.rows[this.count-1][1] : val ]);        
      this.labels.push(label);
      };
//this.average = function () { return ( Math.PI * this.radius * 2 ); };
}

var pingData = new pData();
var ping6Data = new pData();

var traceData = new tData();
var trace6Data = new tData();

/* chart data */
var optionsPing = {
      lines: { show: true},
      legend: { show: true, position: "sw", backgroundOpacity: 0.5 },
      points: { show: true },
      xaxis: { tickDecimals: 0, tickSize: 1 },
      yaxis: { min: 0}
};

var optionsTrace = {
      lines: { show: true },
      legend: { show: true, position: "sw", backgroundOpacity: 0.5 },
      points: { show: true },
      xaxis: {zoomRange: [1, 10], panRange: [0, 30] },
      yaxis: {zoomRange: [10, 1000] },
      zoom: { interactive: true },
      pan: { interactive: true, frameRate: 30 },
      valueLabels: { show: true },
      legend: { position: "nw" }
};

function repaintPlots() {
   var $tabs = $('#tabs').tabs();
   var selected = $tabs.tabs('option', 'selected');
   if(selected == "0" && pingData.changed)  /* ping v4 */
   {
      pingData.changed = 0;

      $.plot(pingPlaceholder, 
	    [ { data: pingData.max, label: "Max ["+ pingData.maxVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.1)", lines: {show: true, fill: 0.1}, points: {show: false}, shadowSize: 0},
	      {	data: pingData.avrgs, label: "Avg ["+ pingData.avrgVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 1)", points: {show: false}},
	      { data: pingData.min, label: "Min ["+ pingData.minVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.3)", lines: {show: true, fill: 0.3}, points: {show: false}, shadowSize: 0},
	      { data: pingData.rows, color: "#2779AA" }],
	    $.extend(true, {}, optionsPing, {
	       xaxis: { min: pingData.count - pingData.maxValues < 0 ? 0 : pingData.count - pingData.maxValues, 
                   max: pingData.count > 10? pingData.count + 1 : 11}
	       }));
	       
	 /* add label with percentage of lost packets */
    pingPlaceholder.append('<div class = "lostPacketsLabelLight">Packet loss: '+pingData.getLostPercent()+'%</div><div class = "lostPacketsLabel">Packet loss: '+pingData.getLostPercent()+'%</div>');
   }
   if(selected == "1" && ping6Data.changed)  /* ping v6 */
   {
      ping6Data.changed = 0;

      $.plot(ping6Placeholder, 
	    [ { data: ping6Data.max, label: "Max ["+ ping6Data.maxVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.1)", lines: {show: true, fill: 0.1}, points: {show: false}, shadowSize: 0},
	      {	data: ping6Data.avrgs, label: "Avg ["+ ping6Data.avrgVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 1)", points: {show: false}},
	      { data: ping6Data.min, label: "Min ["+ ping6Data.minVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.3)", lines: {show: true, fill: 0.3}, points: {show: false}, shadowSize: 0},
	      { data: ping6Data.rows, color: "#2779AA" }],
	    $.extend(true, {}, optionsPing, {
	       xaxis: { min: pingData.count - pingData.maxValues < 0 ? 0 : pingData.count - pingData.maxValues, 
                   max: pingData.count > 10? pingData.count + 1 : 11}
	       }));
	    /* add label with percentage of lost packets */
    ping6Placeholder.append('<div class = "lostPacketsLabelLight">Packet loss: '+pingData.getLostPercent()+'%</div><div class = "lostPacketsLabel">Packet loss: '+pingData.getLostPercent()+'%</div>');
   }
   if((selected == "2" && traceData.changed)||(selected == "3" && trace6Data.changed))  /* traceroute v4, v6 */
   {
      var tdata = selected == "2"? traceData : trace6Data;
      var placeholder = selected == "2"? traceroutePlaceholder : traceroute6Placeholder;
      var axes = selected == "2"? tracerouteAxes : traceroute6Axes;
      var plotCont ;

      tdata.changed = 0;
      
      
      plotCont = $.plot(placeholder, [{ data: tdata.rows, label: "Position", color: "#2779AA" }], $.extend(true, {}, optionsTrace, {
      xaxis: { tickDecimals: 0, tickSize: 1, min: (axes != undefined ? axes.xaxis.min : 0), max: (axes != undefined ? axes.xaxis.max : 30) },
      yaxis: { min: (axes != undefined ? axes.yaxis.min : 0), max: (axes != undefined ? axes.yaxis.max : null) },

      }));

      // Functions for zooming/paning plots
      traceroutePlaceholder.bind('plotpan', function (event, plot) {
	 axes = plot.getAxes();
	 plot.getPlaceholder().find(".valueLabel").remove();
	 plot.getPlaceholder().find(".valueLabelLight").remove();
	 plot.draw();
      });
      traceroutePlaceholder.bind('plotzoom', function (event, plot) {
	 axes = plot.getAxes();
	 plot.getPlaceholder().find(".valueLabel").remove();
	 plot.getPlaceholder().find(".valueLabelLight").remove();
	 plot.draw();
      });
      
      var c = plotCont.offset();
	    c.left = 300;
	    c.top = 100;

      /* buttons for zooming in and out */
      $('<img id="zoomin" src="images/zoomin.png">').appendTo(placeholder).click(function (e) {
            e.preventDefault();
            plotCont.zoom({ center: c });
      });
      $('<img id="zoomout" src="images/zoomout.png">').appendTo(placeholder).click(function (e) {
            e.preventDefault();
            plotCont.zoomOut({ center: c });
      });

   }
   
}

/* bind plot functions to tab changing */
$('#tabs').bind('tabsshow', function() {
   repaintPlots();
   tabsLoading = false;
});

$('#tabs').tabs({
   select: function() {
      if(tabsLoading)
	 return false;
      else
	 return (tabsLoading = true);
   }
});


function plotPing(received, type)
{
   if(received == "")
      return;
   
   var data = type == 4? pingData : ping6Data;   
   var pingTime;
   var npos;
      
   /* connect with previous data */
   data.prevData += received;
   
   /* divide input data into lines and add them as data */
   while((npos = data.prevData.indexOf("\n")) != -1)
   {
      var currentData = data.prevData.substr(0, npos);
      if($.client.os == "Windows") 
      {
         var pingTime = parseFloat(/\d+\.{0,1}\d*ms/i.exec(currentData));
         if(pingTime)
            data.add(pingTime);
         else
         {
            /* control of response keywords */
            if(currentData.indexOf("Vyprsel") != -1 || received.indexOf("neni dostupny") != -1 
               || received.indexOf("timed out") != -1 || received.indexOf("unreachable") != -1 )
            {
               data.add(null);
            }
         }
      }
      else if($.client.os == "Linux") 
      {
         var pingTime = parseFloat(/\d+\.{0,1}\d*\sms/i.exec(currentData));
         if(pingTime)
         {
	    /* check for lost packets */
	    var actPingId = parseInt((/icmp_seq=\d+/i.exec(currentData))[0].substr(9)); 
	    while(actPingId != null && actPingId > ++data.prevId) 
   	       data.add(null);
	    data.add(pingTime);
         }
      }
   	/* store remaining data */
      data.prevData = (data.prevData.substr(npos+1));
   }
  
   /*update plots*/
   repaintPlots();
}

function addTPlotDataWin(row, type)
{
   var data = type == 4? traceData : trace6Data;
   
   var nospaces = row.replace(/\s+/g, ' ');	/* remove multiple spaces */
   var time = parseFloat(/\d+\.{0,1}\d*\sms/i.exec(row));
      
   var fields = nospaces.split(" ");
   var step, label, first = 0, labelPos = 4;
   
   while(fields[first] == "" && first<fields.length-6)
      first++;
   
   if(!(step = parseInt(fields[first])))
      return;
      
   for( j = first; j < fields.length; j++)
      if(fields[j] == "ms") 
         labelPos++;      
   
   label = fields[first + labelPos];
   
   data.add(time, label);
}

function addTPlotDataLin(row, type)
{
   var data = type == 4? traceData : trace6Data;
   var step, label= "", first = 0;
   var time = parseFloat(/\d+\.{0,1}\d*\sms/i.exec(row));

   var nospaces = row.replace(/\s+/g, ' ');	/* remove multiple spaces */
   var fields = nospaces.split(" ");
   
   /* find first column */
   while(fields[first] == "" && first<fields.length-3)
      first++;

   if(!(step = parseFloat(fields[first])))
      return;
   /* check for not comming packets */
   if(fields[first+1] != "*")
      label = fields[first+1];
   else
   {
      if(fields[first+2] != "*")
	 label = fields[first+2];
      else
      {
      	 if(fields[first+3] != "*")
      	    label = fields[first+3];
      }
   }

   data.add(time, label);
}

/* draw plot */
function plotTraceroute(received, type)
{
   if(received == "")
      return;
      
   var data = type == 4? traceData : trace6Data;
   /* connect with previous data */
   data.prevData += received;
   /* prepare data */
   
   if($.client.os == "Windows")
   {
      var npos;
      /* divide input data into lines and add them as data */
      while((npos = data.prevData.indexOf("\n")) != -1)
      {
         addTPlotDataWin(data.prevData.substr(0, npos), type);
         data.prevData = (data.prevData.substr(npos+1));
      }
   }   
   else if($.client.os == "Linux")
   {
      var npos;
      /* divide input data into lines and add them as data */
      while((npos = data.prevData.indexOf("\n")) != -1)
      {
         addTPlotDataLin(data.prevData.substr(0, npos), type);
         data.prevData = (data.prevData.substr(npos+1));
      }
   }
   else
   {
   }

   repaintPlots();
}

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
