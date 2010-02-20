//alert(document.getElementById('conetservDialog').defaultButton);
//alert(document.documentElement.getButton("accept").oncommand);

function focusOnRun(){
  document.getElementById('conetservStartButton').focus();
}

function start(){
  document.getElementById('conetservStartButton').focus();
  var currentText = document.getElementById('conetservTabs').selectedPanel.firstChild;
  currentText.value+=document.getElementById('conetservUrl').value+"\n";
}