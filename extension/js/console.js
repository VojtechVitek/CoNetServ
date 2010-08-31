/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/*
 * function for creating console objects, which are used for output of program
 *
 */
Conetserv.Console = function(div) {
   this.div = document.getElementById(div);   //pointer to div which object takes care of
   this.code = "";            //container for data to be shown in element
   this.err = "";             //error data show in the top of console

   this.prevData = "";            //used for making sure whole line is being written
   this.direction = 0;            //direction of output 0 = downwards 1 = upwards
   this.rows = [];               //variable for storing rows of code
   this.rowCount = 0;            //defines count of rows
   this.maxRows = 0;            //maximum count of rows
   this.percent = 0;

   /* regular expressions */
   this.linux = new Object();
   this.linux.hostname = /([\w\-]+\.)+([a-z]+)/ig //\(([0-9\i]*)\).*/i
   this.linux.ip = /(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})/ig
   this.linux.time = /(?:\/?(\d+\.?\d*))+\s?ms(ec){0,1}/ig  // format: xx.yy/zz.yy ms

   /* for inputTimerErr */
   this.touched = true;
   this.timedErr = false;
   this.timer = false;
   
   
   //gets color from string containing number at the begining
   //0% green 50% yellow 100% red
   //returns final html element
   this.getColor = function(input){
      var color;
      // 0.2 = 100(percent)/500(maxvalue)
      var numberR = /\d+\.?\d*/g;
      var result = input.match(numberR);
      var finished = [];
      
      for (var key in result) {
         var nb = result[key];
         
         var percent = parseFloat(nb)*0.2;
         if (percent <= 50) {
            color = parseInt(percent/50*196);

            color = (color<16.0 ? "#0" : "#") + (color).toString(16) + "B000";
         }
         else if(percent<100)
            color = "#B0" + (parseInt(196-(percent-50)/50*196)).toString(16) + "00";
         else 
            color = "#B00000";

          if(!finished[nb]) {
             input = input.replace(nb, '<span style="color:' + color + '">' + nb + '</span>');
             finished[nb] = true;
          }
      }
      return input;
   }
   
   //sets div for output - accepts name of div
   this.setDiv = function(div){
      this.div = document.getElementById(div);
   }

   //adds text to console, automaticaly stylizes for ping, traceroute, etc
   this.add = function(text){
      /* set touched to true and clear timed error if present */
      this.touched = true;
      if(this.timedErr) {
         this.clearErr();
      }

      this.prevData += text;
      var npos = 0;
      this.prevData = this.prevData.replace(/(\r?\n)+/gi, "\n");

      while((npos = this.prevData.indexOf("\n")) != -1 ) {
         this.rowCount++;
         var row = this.colourLine(this.prevData.substr(0, npos+1));

         /* In case of first line being only \n skip it */
         if(this.rowCount == 1 && row == "<br />"){
            this.rowCount--;
             this.prevData = (this.prevData.substr(npos+1));
            continue;
         }
         
         if(this.direction) { //writing to the begining of container
            this.rows.unshift(row);
            if(this.maxRows && this.rowCount > this.maxRows) {
               this.rows.pop();
            }
         } else { //writing to the end of container
            this.rows.push(row);
            if(this.maxRows && this.rowCount > this.maxRows) {
               this.rows.shift();
            }
         }
         
         this.code = this.rows.join("");
         /* store remaining data */
         this.prevData = (this.prevData.substr(npos+1));
      }
      
      /*   
      var tmp = document.createTextNode(this.code);
      this.div.appendChild(tmp);
      return;
      */  
      
      this.repaint();
   }

   // sets error in the console
   this.setErr = function(text) {
      var errText = text.replace(/((\r)*\n)+/g,"<br />");
      this.err = '<div class="ui-state-highlight ui-corner-all" style="padding: 0em 1em; margin-top:5px;"> \
         <p><span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>'+errText+'</p></div>';

      this.repaint();
   }

   this.clearErr = function() {
      this.err = "";
      this.repaint();
   }
   
   //colours all items in line
   this.colourLine = function(line) {
      var coloured = line.replace(/((\r)*\n)+/g,"<br />");

      coloured = coloured.replace(this.linux.hostname,"<span class=\"color1\">$&</span>"); 
      coloured = coloured.replace(this.linux.ip,"<span class=\"color2\">$1</span>"); 
      coloured = coloured.replace(this.linux.time, this.getColor);
      
      return coloured;
   }

   //clears console
   this.clear = function(){
      /* clear timer of inputTimerErr */
      clearTimeout(this.timer);
      
      this.err = "";
      this.code = "";
      this.prevData = "";
      this.rows = [];
      this.rowCount = 0;
      this.repaint();
   }
   
   //repaints whole console
   this.repaint = function(){
      if(this.div)
         this.div.innerHTML = this.err + this.code;
   }

   /*
    * Checks if any input is written inside, otherwise sets error
    * Object must be same as parameter object
    * @param object defines which console will be setErr run in
    * @param time seconds after which error message is written inside
    * @param msg error message to be written inside
    */
   this.inputTimerErr = function(object, time, msg) {
      this.touched = 0;
      this.timer = setTimeout(object + '._inputTimerSetErr("' + msg + '")', time * 1000);
   },

   /*
    * Private function to be called by inputTimerErr, which checks if
    * console has been changed since start of timer
    * @param msg msg to be shown in console
    */
   this._inputTimerSetErr = function(msg) {
      if(!this.touched) {
         this.setErr(msg);
         this.timedErr = true;
      }
   }


}
