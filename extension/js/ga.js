
(function() {
  var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
  ga.src = 'https://ssl.google-analytics.com/ga.js';
  (document.getElementsByTagName('head')[0] || document.getElementsByTagName('body')[0]).appendChild(ga);
})();

$(document).ready(function(){
   var _gaq = _gaq || [];
   _gaq.push(['_setAccount', 'UA-16884739-1']);
   _gaq.push(['_trackPageview']);

   $(".radio-tab-label").click(function() {
      _gaq.push(['_trackEvent', 'button' + this.id, 'clicked']);
   });

   $(".tab-header-button").click(function() {
      _gaq.push(['_trackEvent', 'button' + this.parentNode.id, 'clicked']);
   });

   _gaq.push(['_trackPageview']);
   
});
