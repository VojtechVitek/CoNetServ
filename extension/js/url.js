
/** 
 * Object for handling urls - filtering, ...
 *
 */
var url = {
   //regexps for checking and parsing data
   ipv4: /^(f|ht)tp[s]{0,1}:[/]{2}\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/ig,
   http: /((http|https|ftp):\/\/|www.)?([^/ \.]+\.)+([^/ ]+)(\/[^ ]*)*/ig,
   parseDns: /(http:\/\/|https:\/\/|ftp:\/\/)?([a-z0-9]+\.)+([a-z0-9]+)/i,

   //stores url value
   //format: http://www.nic.cz/blabla
   //	    ^ pre  ^ value   ^ past
   pre: "",
   value: "",
   past: "",

   //Checks address for validity to ping, traceroute,...
   checkAddress: function(addr)
   {
      return this.ipv4.test(addr) || this.http.test(addr);
   },
   
   //sets url address of object
   set: function(addr)
   {
      if(!this.checkAddress(addr))
      {
	 this.pre = this.past = this.value = "";
      }
      else
      {
	 //erase http, ftp, ...
	 var result = this.parseDns.exec(addr);
	 if(result[1])
	 {
	    this.pre = result[1];
	    this.value = result[0].substr(result[1].length);
	    this.past = addr.substr(result[0].length);
	 }
	 else this.value = result[0];
	 return;
      }
   }

}

function hostnameFromUrl(url) {
    return url.match(/:\/\/(.[^/]+)/)[1];
}
