/* object for handling google map */

/*!! Object is shown from plot.js function bind tabshow */

var Map = {
   latitude: 0,
   langitute: 0,
   latlng: 0,
   enabled: 0,
   map:0,
   options: 0,

   marker: 0,
   
   inicialize: function(lat, lan) {
      if(!lat || !lan)
	 return;
      this.enabled = 1;
      this.latitude = parseFloat(lat);
      this.langitude = parseFloat(lan);
   },
   
   show: function(){
      if(!this.enabled)
	 return;
      //check if right tab is selected
      var $tabs = $('#tabs').tabs();
      var selected = $tabs.tabs('option', 'selected');
      if(selected == "6")
      {
	 this.latlng = new google.maps.LatLng(41.033, 28.95);//this.latitude, this.langitude);
	 
	 this.options = {
	    zoom: 10,
	    center: this.latlng,
	    mapTypeId: google.maps.MapTypeId.SATELLITE
	 };

	 if(!this.map)
	    this.map = new google.maps.Map(document.getElementById("mapPlaceholder"), this.options);
	 
	 this.marker = new google.maps.Marker({
	    position: this.latlng, 
	    map: this.map,
	    title:"You are here!"
	 });   
      }
   }

};