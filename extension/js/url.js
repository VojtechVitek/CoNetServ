
/** 
 * Object for handling urls - filtering, ...
 *
 */
var url = {
   //regexps for checking and parsing data
   ip: /(http:\/\/|https:\/\/|ftp:\/\/)?(?:(?:(?:(?:[\w]{1,4}(?::[a-f0-9]{1,4}){7})|(?:(?!(?:.*[a-f0-9](?::|$)){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)))|(?:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?))?(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3}))$/i,
   http: /(http:\/\/|https:\/\/|ftp:\/\/)?([\w\-]+\.)+([\w]+)/i,
   parseDns: /(http:\/\/|https:\/\/|ftp:\/\/)?([\w\-]+\.)+([\w]+)/i,
   parseIp: /(http:\/\/|https:\/\/|ftp:\/\/)?([\w\-\.])/i,

   //stores url value
   //format: http://www.nic.cz/blabla
   //	    ^ pre  ^ value   ^ past
   pre: "",
   value: "",
   past: "",

   //Checks address for validity to ping, traceroute,...
   checkAddress: function(addr)
   {
      return this.http.test(addr) || this.ip.test(addr) ;
   },
   
   //sets url address of object
   //on success returns 1, on failure 0
   set: function(addr)
   {
      this.pre = this.past = this.value = "";
      if(!this.checkAddress(addr))
      {
	 return 0;
      }
      else
      {
	 //erase http, ftp, ...
	 var result = this.parseDns.exec(addr);
	 if(!result) 
	    result = this.parseIp.exec(addr);
	 if(result[1])
	 {
	    this.pre = result[1];
	    this.value = result[0].substr(result[1].length);
	    this.past = addr.substr(result[0].length);
	 }
	 else this.value = result[0];
	 if(!this.value || this.value == "")
	    return 0;
	 return 1;
      }
   }

}

function hostnameFromUrl(url) {
    return url.match(/:\/\/(.[^/]+)/)[1];
}
