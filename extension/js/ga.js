
(function() {
 var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
 ga.src = 'https://ssl.google-analytics.com/ga.js';
 var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
})();

$(document).ready(function(){
   var _gaq = _gaq || [];
   _gaq.push(['_setAccount', 'UA-16884739-1']);
   _gaq.push(['_trackPageview']);

   $(".radio-tab-label").click(function() {
      _gaq.push(['_trackEvent', 'button' + this.id, 'clicked']);
   });

_gaq.push(['_trackPageview']);
   
});
