/**
 * Object for plotting of graphs for services like ping and traceroute
 */
var Plot = {
   /* initialize data */

   /* placeholders for plots */
   pingPlaceholder : false,
   ping6Placeholder : false,
   tracertPlaceholder : false,
   tracert6Placeholder : false,

   /* data for plotting */
   pingData : new pData(),
   ping6Data : new pData(),
   traceData : new tData(),
   trace6Data : new tData(),

   tracertAxes : false,
   tracert6Axes : false,

   /* options for flot library - ping and traceroute */
   optionsPing : {
      lines: {show: true},
      legend: {show: true, position: "sw", backgroundOpacity: 0.5},
      points: {show: true},
      xaxis: {tickDecimals: 0, tickSize: 1},
      yaxis: {min: 0}
   },

   optionsTrace : {
      lines: {show: true},
      legend: {show: true, position: "nw", backgroundOpacity: 0.5},
      points: {show: true},
      xaxis: {zoomRange: [1, 10], panRange: [0, 30]},
      yaxis: {zoomRange: [10, 1000]},
      zoom: {interactive: true},
      pan: {interactive: true, frameRate: 30},
      valueLabels: {show: true}
   },

   /**
    * sets all the parameters, which needs to be applied after document.ready
    */
   inicialize : function() {
      this.pingPlaceholder = $("#local-ping-placeholder");
      this.ping6Placeholder = $("#local-ping6-placeholder");
      this.tracertPlaceholder = $("#local-tracert-placeholder");
      this.tracert6Placeholder = $("#local-tracert6-placeholder");
   },

   /**
    * repaints all the plots defined in this file depending on actual data
    */
   repaint : function() {
      var $tabs = $('#tabs').tabs();
      var selected = $tabs.tabs('option', 'selected');

      /* check for local services tab selected for repainting */
      if(selected == "0")
      {
         var active = $("#local-services input:radio:checked").val();

         /* ping v4 */
         if(active == 'local-ping-div' && this.pingData.changed) {

            this.pingData.changed = 0;

            $.plot(this.pingPlaceholder,
             [ {data: this.pingData.max, label: "Max ["+ this.pingData.maxVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.1)", lines: {show: true, fill: 0.1}, points: {show: false}, shadowSize: 0},
               {data: this.pingData.avrgs, label: "Avg ["+ this.pingData.avrgVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 1)", points: {show: false}},
               {data: this.pingData.min, label: "Min ["+ this.pingData.minVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.3)", lines: {show: true, fill: 0.3}, points: {show: false}, shadowSize: 0},
               {data: this.pingData.rows, color: "#2779AA"} ],
             $.extend(true, {}, this.optionsPing, {
                xaxis: {min: this.pingData.count - this.pingData.maxValues < 0 ? 0 : this.pingData.count - this.pingData.maxValues,
                         max: this.pingData.count > 10? this.pingData.count + 1 : 11}
             }));

             /* add label with percentage of lost packets */
             this.pingPlaceholder.append('<div class = "lostPacketsLabelLight">Packet loss: '+this.pingData.getLostPercent()+'%</div><div class = "lostPacketsLabel">Packet loss: '+this.pingData.getLostPercent()+'%</div>');
         }

         /* ping v6 */
         if(active == 'local-ping6-div' && this.ping6Data.changed) {
            this.ping6Data.changed = 0;

            $.plot(this.ping6Placeholder,
             [ {data: this.ping6Data.max, label: "Max ["+ this.ping6Data.maxVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.1)", lines: {show: true, fill: 0.1}, points: {show: false}, shadowSize: 0},
               {data: this.ping6Data.avrgs, label: "Avg ["+ this.ping6Data.avrgVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 1)", points: {show: false}},
               {data: this.ping6Data.min, label: "Min ["+ this.ping6Data.minVal.toFixed(2) +"ms]", color: "rgba(103, 170, 238, 0.3)", lines: {show: true, fill: 0.3}, points: {show: false}, shadowSize: 0},
               {data: this.ping6Data.rows, color: "#2779AA"}],
            $.extend(true, {}, this.optionsPing, {
                xaxis: {min: this.pingData.count - this.pingData.maxValues < 0 ? 0 : this.pingData.count - this.pingData.maxValues,
                         max: this.pingData.count > 10? this.pingData.count + 1 : 11}
                }));
             /* add label with percentage of lost packets */
            this.ping6Placeholder.append('<div class = "lostPacketsLabelLight">Packet loss: '+this.pingData.getLostPercent()+'%</div><div class = "lostPacketsLabel">Packet loss: '+this.pingData.getLostPercent()+'%</div>');
         }

         /* traceroute */
         if((active == 'local-tracert-div' && this.traceData.changed)||(active =='local-tracert6-div' && this.trace6Data.changed))  /* tracert v4, v6 */
         {
            var tdata = active == 'local-tracert-div'? this.traceData : this.trace6Data;
            var placeholder = active == 'local-tracert-div'? this.tracertPlaceholder : this.tracert6Placeholder;
            var plotCont;

            tdata.changed = 0;


            plotCont = $.plot(placeholder, [{data: tdata.rows, label: "Position", color: "#2779AA"}], $.extend(true, {}, this.optionsTrace, {
              xaxis: {tickDecimals: 0, tickSize: 1 }
            }));

            // Functions for zooming/paning plots
            /*this.tracertPlaceholder.bind('plotpan', function (event, plot) {
               axes = plot.getAxes();
               plot.getPlaceholder().find(".valueLabel").remove();
               plot.getPlaceholder().find(".valueLabelLight").remove();
               plot.draw();
            });
            this.tracertPlaceholder.bind('plotzoom', function (event, plot) {
               axes = plot.getAxes();
               plot.getPlaceholder().find(".valueLabel").remove();
               plot.getPlaceholder().find(".valueLabelLight").remove();
               plot.draw();
            });*/

            var c = plotCont.offset();
             c.left = 300;
             c.top = 100;

            /* buttons for zooming in and out */
            /*$('<img id="zoomin" src="images/zoomin.png">').appendTo(placeholder).click(function (e) {
                  e.preventDefault();
                  plotCont.zoom({center: c});
            });
            $('<img id="zoomout" src="images/zoomout.png">').appendTo(placeholder).click(function (e) {
                  e.preventDefault();
                  plotCont.zoomOut({center: c});
            });*/
         }
      }
   },

   /**
    * Paints plot for ping service with received data added to actual data.
    * @param received Defines data, which will be analyzed and stored to
    * object structures
    * @param type Numeral value for defining, which version of ping we are
    * plotting ( 4 for v.4 6 for v.6 ).
    */
   plotPing : function(received, type) {
      if(received == "")
         return;

      var data = type == 4? this.pingData : this.ping6Data;
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
            var regHead = /^\w+\s.*?\d+\.\d+\.\d+\.\d+.*/
            alert(regHead.exec(currentData));
            pingTime = parseFloat(/\d+\.{0,1}\d*ms/i.exec(currentData));
            if(pingTime)
               data.add(pingTime);
            else
            {
               //!! TODO check on windows!!
               /* control of response keywords */
               if(!regHead.exec(currentData))
               {
                  data.add(null);
               }
            }
         }
         else if($.client.os == "Linux")
         {
            pingTime = parseFloat(/\d+\.{0,1}\d*\sms/i.exec(currentData));
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
      this.repaint();
   },

   /**
    * Paints plot for tracert service with received data added to actual data.
    * @param received Defines data, which will be analyzed and stored to
    * object structures
    * @param type Numeral value for defining, which version of ping we are
    * plotting ( 4 for v.4 6 for v.6 ).
    */
   plotTracert : function(received, type) {
      if(received == "")
         return;

      var data = type == 4? this.traceData : this.trace6Data;
      /* connect with previous data */
      data.prevData += received;
      /* prepare data */

      if($.client.os == "Windows")
      {
         var npos;
         /* divide input data into lines and add them as data */
         while((npos = data.prevData.indexOf("\n")) != -1)
         {
            this._addTPlotDataWin(data.prevData.substr(0, npos), type);
            data.prevData = (data.prevData.substr(npos+1));
         }
      }
      else if($.client.os == "Linux")
      {
         var npos;
         /* divide input data into lines and add them as data */
         while((npos = data.prevData.indexOf("\n")) != -1)
         {
            this._addTPlotDataLin(data.prevData.substr(0, npos), type);
            data.prevData = (data.prevData.substr(npos+1));
         }
      }
      else
      {
      }

      this.repaint();
   },


   /**
    * Private function for adding traceroute data on windows platform
    * @param row One row of data to be added to data structures of object.
    * @param type Numeral value for defining, which version of ping we are
    * plotting ( 4 for v.4 6 for v.6 ).
    */
   _addTPlotDataWin : function(row, type) {
      var data = type == 4? this.traceData : this.trace6Data;

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
   },

   /**
    * Private function for adding traceroute data on linux platform
    * @param row One row of data to be added to data structures of object.
    * @param type Numeral value for defining, which version of ping we are
    * plotting ( 4 for v.4 6 for v.6 ).
    */
   _addTPlotDataLin : function(row, type)
   {
      var data = type == 4? this.traceData : this.trace6Data;
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
}

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
      this.avrgVal = this.sum/(this.count-this.lost);
      
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

   /* sets data for reploting */
   this.touch = function (){this.changed = true;};

   /* resets object to initial state */
   this.reset = function() {
      this.count = this.prevId = this.sum = this.minVal = this.maxVal = this.avrgVal = this.actVal = this.lost = 0;
      this.rows = [];this.min = [];this.max = [];this.avrgs=[];this.prevData = "";this.changed = 1;
   };

   /* returns percentage of lost packets */
   this.getLostPercent = function (){
      var perc = this.lost > 0 ? this.lost/this.count*100 : 0;
      return perc.toFixed(2);
   };
}

/* tracert data object */
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

   /* sets data for reploting */
   this.touch = function (){this.changed = true;};
   
   /* resets object to initial state */
   this.reset = function() {
      this.count = this.prevId = 0;
      this.rows = [];this.labels = [];this.prevData = "";this.changed = 1;
   };
}

/* bind plot functions to tab changing */
$(document).ready(function()
{
   $("#tabs").bind('tabsshow', function() 
   {
      Plot.pingData.touch();
      Plot.ping6Data.touch();
      Plot.traceData.touch();
      Plot.trace6Data.touch();
      Plot.repaint();
   });

   $("#tabs").bind('select', function() 
   {
      return true;
   });

   Plot.repaint();
}
);