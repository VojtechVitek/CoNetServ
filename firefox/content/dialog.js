//alert(document.getElementById('conetservDialog').defaultButton);
//alert(document.documentElement.getButton("accept").oncommand);

var conetserv = document.getElementById("conetserv");
var result = document.getElementById("result");
var interval;

function start() {
   document.getElementById('conetservStartButton').focus();
   try {
      conetserv.startPing("fres-solutions.com");
      interval = window.setInterval("read()", 1000);
   }
   catch(e) {
      alert(e);
   }
   result.value = "";
}

function read() {
   try {
      document.getElementById('conetservTabs').selectedPanel.firstChild.value = document.getElementById('conetservTabs').selectedPanel.firstChild.value + conetserv.readPing();
   }
   catch(e) {
      alert(e);
   }
}

function stop()
{
   try {
       conetserv.stopPing();
   }
   catch(e) {
      alert(e);
   }
   window.clearInterval(interval);
}

function focusOnRun(){
  document.getElementById('conetservStartButton').focus();
}
