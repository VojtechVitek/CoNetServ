/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

/*
 * function for creating console objects, which are used for output of program
 *
 */
Conetserv.Console = function(div) {
   this.div = document.getElementById(div);   //pointer to div which object takes care of
   this.code = "";            //container for data to be shown in element

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
   this.linux.time = /\d+\.?\d*\s?ms(ec){0,1}/ig
   
   
   //gets color from string containing number at the begining
   //0% green 50% yellow 100% red
   //returns final html element
   this.getColor = function(input){
      var color;
      // 0.2 = 100(percent)/500(maxvalue)
      var percent = parseFloat(input)*0.2;
      if (percent <= 50) {
         color = parseInt(percent/50*196);

         color = (color<16.0 ? "#0" : "#") + (color).toString(16) + "B000";
      }
      else if(percent<100)
         color = "#B0" + (parseInt(196-(percent-50)/50*196)).toString(16) + "00";
      else 
         color = "#B00000";

      return '<span style="color:' + color + '">' + input + '</span>';
   }
   
   //sets div for output - accepts name of div
   this.setDiv = function(div){
      this.div = document.getElementById(div);
   }

   //adds text to console, automaticaly stylizes for ping, traceroute, etc
   this.add = function(text){
      this.prevData += text;
      var npos = 0;

      while((npos = this.prevData.indexOf("\n")) != -1 ) {
         this.rowCount++;
         var row = this.colourLine(this.prevData.substr(0, npos+1));
         
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
      
      this.div.innerHTML = this.code;
   }
   
   //colours all items in line
   this.colourLine = function(line) {
      var coloured = line.replace(/(\r)*\n/g,"<br />");

      coloured = coloured.replace(this.linux.hostname,"<span class=\"color1\">$&</span>"); 
      coloured = coloured.replace(this.linux.ip,"<span class=\"color2\">$1</span>"); 
      coloured = coloured.replace(this.linux.time, this.getColor);
      
      return coloured;
   }

   //clears console
   this.clear = function(){
      this.code = "";
      this.prevData = "";
      this.rows = [];
      this.rowCount = 0;
      this.repaint();
   }
   
   //repaints whole console
   this.repaint = function(){
      this.div.innerHTML = this.code;
   }

}
