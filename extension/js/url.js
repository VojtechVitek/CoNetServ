/* Check CoNetServ object */
if(!Conetserv) var Conetserv = {};

//Javascript UTF16 converter created by some@domain.name
//This implementation is released to public domain
Conetserv.utf16 = {
    decode:function(input){
        var output = [], i=0, len=input.length,value,extra;
        while (i < len) {
                value = input.charCodeAt(i++);
                if ((value & 0xF800) === 0xD800) {
                        extra = input.charCodeAt(i++);
                        if ( ((value & 0xFC00) !== 0xD800) || ((extra & 0xFC00) !== 0xDC00) ) {
                                throw new RangeError("UTF-16(decode): Illegal UTF-16 sequence");
                        }
                        value = ((value & 0x3FF) << 10) + (extra & 0x3FF) + 0x10000;
                }
                output.push(value);
        }
        return output;
    },
    encode:function(input){
        var output = [], i=0, len=input.length,value;
        while (i < len) {
                value = input[i++];
                if ( (value & 0xF800) === 0xD800 ) {
                        throw new RangeError("UTF-16(encode): Illegal UTF-16 value");
                }
                if (value > 0xFFFF) {
                        value -= 0x10000;
                        output.push(String.fromCharCode(((value >>>10) & 0x3FF) | 0xD800));
                        value = 0xDC00 | (value & 0x3FF);
                }
                output.push(String.fromCharCode(value));
        }
        return output.join("");
    }
}

//Javascript Punycode converter derived from example in RFC3492.
//This implementation is created by some@domain.name and released to public domain
Conetserv.Punycode = new function Punycode() {
    var initial_n = 0x80;
    var initial_bias = 72;
    var delimiter = "\x2D";
    var base = 36;
    var damp = 700;
    var tmin=1;
    var tmax=26;
    var skew=38;

    var maxint = 0x7FFFFFFF;
    // decode_digit(cp) returns the numeric value of a basic code
    // point (for use in representing integers) in the range 0 to
    // base-1, or base if cp is does not represent a value.

    function decode_digit(cp) {
        return  cp - 48 < 10 ? cp - 22 :  cp - 65 < 26 ? cp - 65 : cp - 97 < 26 ? cp - 97 : base;
    }

    // encode_digit(d,flag) returns the basic code point whose value
    // (when used for representing integers) is d, which needs to be in
    // the range 0 to base-1.  The lowercase form is used unless flag is
    // nonzero, in which case the uppercase form is used.  The behavior
    // is undefined if flag is nonzero and digit d has no uppercase form.

    function encode_digit(d, flag) {
        return d + 22 + 75 * (d < 26) - ((flag != 0) << 5);
        //  0..25 map to ASCII a..z or A..Z
        // 26..35 map to ASCII 0..9
    }
    //** Bias adaptation function **
    function adapt(delta, numpoints, firsttime ) {
        var k;
        delta = firsttime ? Math.floor(delta / damp) : (delta >> 1);
        delta += Math.floor(delta / numpoints);

        for (k = 0;  delta > (((base - tmin) * tmax) >> 1);  k += base) {
                delta = Math.floor(delta / ( base - tmin ));
        }
        return Math.floor(k + (base - tmin + 1) * delta / (delta + skew));
    }

    // encode_basic(bcp,flag) forces a basic code point to lowercase if flag is zero,
    // uppercase if flag is nonzero, and returns the resulting code point.
    // The code point is unchanged if it  is caseless.
    // The behavior is undefined if bcp is not a basic code point.

    function encode_basic(bcp, flag) {
        bcp -= (bcp - 97 < 26) << 5;
        return bcp + ((!flag && (bcp - 65 < 26)) << 5);
    }

    // Main decode
    this.decode=function(input,preserveCase) {
        // Dont use uft16
        var output=[];
        var case_flags=[];
        var input_length = input.length;

        var n, out, i, bias, basic, j, ic, oldi, w, k, digit, t, len;

        // Initialize the state:

        n = initial_n;
        i = 0;
        bias = initial_bias;

        // Handle the basic code points:  Let basic be the number of input code
        // points before the last delimiter, or 0 if there is none, then
        // copy the first basic code points to the output.

        basic = input.lastIndexOf(delimiter);
        if (basic < 0) basic = 0;

        for (j = 0;  j < basic;  ++j) {
                if(preserveCase) case_flags[output.length] = ( input.charCodeAt(j) -65 < 26);
                if ( input.charCodeAt(j) >= 0x80) {
                        throw new RangeError("Illegal input >= 0x80");
                }
                output.push( input.charCodeAt(j) );
        }

        // Main decoding loop:  Start just after the last delimiter if any
        // basic code points were copied; start at the beginning otherwise.

        for (ic = basic > 0 ? basic + 1 : 0;  ic < input_length; ) {

                // ic is the index of the next character to be consumed,

                // Decode a generalized variable-length integer into delta,
                // which gets added to i.  The overflow checking is easier
                // if we increase i as we go, then subtract off its starting
                // value at the end to obtain delta.
                for (oldi = i, w = 1, k = base;  ;  k += base) {
                        if (ic >= input_length) {
                                throw RangeError ("punycode_bad_input(1)");
                        }
                        digit = decode_digit(input.charCodeAt(ic++));

                        if (digit >= base) {
                                throw RangeError("punycode_bad_input(2)");
                        }
                        if (digit > Math.floor((maxint - i) / w)) {
                                throw RangeError ("punycode_overflow(1)");
                        }
                        i += digit * w;
                        t = k <= bias ? tmin : k >= bias + tmax ? tmax : k - bias;
                        if (digit < t) { break; }
                        if (w > Math.floor(maxint / (base - t))) {
                                throw RangeError("punycode_overflow(2)");
                        }
                        w *= (base - t);
                }

                out = output.length + 1;
                bias = adapt(i - oldi, out, oldi === 0);

                // i was supposed to wrap around from out to 0,
                // incrementing n each time, so we'll fix that now:
                if ( Math.floor(i / out) > maxint - n) {
                        throw RangeError("punycode_overflow(3)");
                }
                n += Math.floor( i / out ) ;
                i %= out;

                // Insert n at position i of the output:
                // Case of last character determines uppercase flag:
                if (preserveCase) { case_flags.splice(i, 0, input.charCodeAt(ic -1)  -65 < 26);}

                output.splice(i, 0, n);
                i++;
        }
        if (preserveCase) {
                for (i = 0, len = output.length; i < len; i++) {
                        if (case_flags[i]) {
                                output[i] = (String.fromCharCode(output[i]).toUpperCase()).charCodeAt(0);
                        }
                }
        }
        return Conetserv.utf16.encode(output);
    };

    //** Main encode function **

    this.encode = function (input,preserveCase) {
        //** Bias adaptation function **

        var n, delta, h, b, bias, j, m, q, k, t, ijv, case_flags;

        if (preserveCase) {
                // Preserve case, step1 of 2: Get a list of the unaltered string
                case_flags = Conetserv.utf16.decode(input);
        }
        // Converts the input in UTF-16 to Unicode
        input = Conetserv.utf16.decode(input.toLowerCase());
        //input = Conetserv.utf16.decode(input);

        var input_length = input.length; // Cache the length

        if (preserveCase) {
                // Preserve case, step2 of 2: Modify the list to true/false
                for (j=0; j < input_length; j++) {
                        case_flags[j] = input[j] != case_flags[j];
                }
                }

        var output=[];


        // Initialize the state:
        n = initial_n;
        delta = 0;
        bias = initial_bias;

        // Handle the basic code points:
        for (j = 0;  j < input_length;  ++j) {
                if ( input[j] < 0x80) {
                        output.push(
                                String.fromCharCode(
                                        case_flags ? encode_basic(input[j], case_flags[j]) : input[j]
                                )
                        );
                }
        }

        h = b = output.length;

        // h is the number of code points that have been handled, b is the
        // number of basic code points

        if (b > 0) output.push(delimiter);

        // Main encoding loop:
        //
        while (h < input_length) {
                // All non-basic code points < n have been
                // handled already. Find the next larger one:

                for (m = maxint, j = 0;  j < input_length;  ++j) {
                        ijv = input[j];
                        if (ijv >= n && ijv < m) m = ijv;
                }

                // Increase delta enough to advance the decoder's
                // <n,i> state to <m,0>, but guard against overflow:

                if (m - n > Math.floor((maxint - delta) / (h + 1))) {
                        throw RangeError("punycode_overflow (1)");
                }
                delta += (m - n) * (h + 1);
                n = m;

                for (j = 0;  j < input_length;  ++j) {
                        ijv = input[j];

                        if (ijv < n ) {
                                if (++delta > maxint) return Error("punycode_overflow(2)");
                        }

                        if (ijv == n) {
                                // Represent delta as a generalized variable-length integer:
                                for (q = delta, k = base;  ;  k += base) {
                                        t = k <= bias ? tmin : k >= bias + tmax ? tmax : k - bias;
                                        if (q < t) break;
                                        output.push( String.fromCharCode(encode_digit(t + (q - t) % (base - t), 0)) );
                                        q = Math.floor( (q - t) / (base - t) );
                                }
                                output.push( String.fromCharCode(encode_digit(q, preserveCase && case_flags[j] ? 1:0 )));
                                bias = adapt(delta, h + 1, h == b);
                                delta = 0;
                                ++h;
                        }
                }

                ++delta, ++n;
        }
        return output.join("");
    }
}();


/**
 * Object for handling urls - filtering, ...
 *
 */
Conetserv.Url = {
   /**
    * Protocol regexp
    * @example https://user:passwd@hostname:port/dir/index.html
    * @returns true of false (test mode)
    * @returns hostname string or null string (exec mode)
    */
   proto: /^(?:(?:(?:http|ftp)s?):\/\/)?(?:[a-z-]+(?:[^@]*)?@)?(?:\[)?([^\[\]\/]*)(?:\])?(?:[:][0-9]+)?(?:\/.*)?$/i,

   /**
    * IPv6 address regexp
    * @example 2001:0db8:85a3:08d3:1319:8a2e:0370:7348
    * @returns true or false (test mode)
    * @returns valid address string or null string (exec mode)
    */
   ipv6_full: /^([a-f0-9]{1,4}(?::[a-f0-9]{1,4}){7})$/i,
   ipv6_compressed: /^((?!(?:.*[a-f0-9](?::|$)){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)$/i,
   ipv6_mapped_ipv4_full: /^((?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?))$/i,
   ipv6_mapped_ipv4_compressed: /^(?:(?!(?:.*[a-f0-9]:){6,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,4})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,4}:)?(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(?:\.(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])){3})$/i,

   /**
    * IPv4 address regexp
    * @example 127.0.0.1
    * @returns true or false (test mode)
    * @returns valid address string or null string (exec mode)
    */
   ipv4: /^((?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3})$/i,

   /**
    * Special address regexp
    * @example localhost
    * @returns true or false (test mode)
    * @returns valid address string or null string (exec mode)
    */
   special_address: /^(localhost)$/i,

   /**
    * URL hostname regexp (Pure ASCII)
    * @example server.example.com
    * @returns true or false (test mode)
    * @returns hostname string or null string (exec mode)
    */
   ascii_url: /^((?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\.)+(?:com|edu|biz|gov|in(?:t|fo)|mil|net|org|[a-z]{2}))$/i,

   /**
    * URL hostname regexp (Punycode)
    * @example www.počítač.háčkyčárky.укр
    */
   punycodeAscii: /^(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])+)$/i,
   punycodeAsciiTld: /^(?:com|edu|biz|gov|in(?:t|fo)|mil|net|org|[a-z]{2})$/i,
   // NOTE: Unicode characters (eg. \X or \P{M}\p{M}*) NOT (YET) IMPLEMENTED IN JAVASCRIPT!?
   punycodeUtf: /^(?:.+)$/i,
   // FIXME: Convert to \uXXXX equivalents, it's probably not working in current JavaScript
   punycodeUtfTld: /^(?:бг|рф|укр)$/i,

   /**
    * Get domain from hostname
    */
   domainFromHostname: /^.*?([^.]+\.(?:(com|co|org|gov|net|)\.)?[^.]+)$/i,

   /*
    * Parsed values
    */
   hostname: '', // www.example.com or 127.0.0.1
   domain: '', // example.com or example.co.uk

   /**
    * Test url/ip address and parse it's hostname
    * @returns true or false
    */
   set: function(addr) {

      var result;

      this.hostname = '';
      this.domain = '';

      // Parse hostname string
      if (this.proto.test(addr)){
         addr = this.proto.exec(addr)[1];
      }

      // comment - parse horse name
      if (this.ipv6_full.test(addr)) {
         // Full IPv6
         this.hostname = this.ipv6_full.exec(addr)[1];
         this.domain = this.hostname;
      } else if (this.ipv6_compressed.test(addr)) {
         // Compressed IPv6
         this.hostname = this.ipv6_compressed.exec(addr)[1];
         this.domain = this.hostname;
      } else if (this.ipv6_mapped_ipv4_full.test(addr)) {
         // Full IPv4 mapped to IPv6
         this.hostname = this.ipv6_mapped_ipv4_full.exec(addr)[1];
         this.domain = this.hostname;
      } else if (this.ipv6_mapped_ipv4_compressed.test(addr)) {
         // Compressed IPv4 mapped to IPv6
         this.hostname = this.ipv6_mapped_ipv4_compressed.exec(addr)[1];
         this.domain = this.hostname;
      } else if (this.ipv4.test(addr)) {
         // IPv4
         this.hostname = this.ipv4.exec(addr)[1];
         this.domain = this.hostname;
      } else if (this.special_address.test(addr)) {
         // Special address
         this.hostname = this.special_address.exec(addr)[1];
         this.domain = this.hostname;
      } else if (this.ascii_url.test(addr)) {
         // ASCII url?
         this.hostname = this.ascii_url.exec(addr)[1];
         // Parse domain from hostname string
         if (this.domainFromHostname.test(this.hostname)){
            this.domain = this.domainFromHostname.exec(this.hostname)[1];
         }
      }
      else {
         // Regular Punycode url?
         // -> try to convert to ASCII
         var arr = addr.split('.');
         var ascii = '';
         if (arr.length < 2)
            return false;
         // try to convert each label of the URL
         for (var i = 0; i < arr.length - 1; ++i) {
            if (this.punycodeAscii.test(arr[i]))
               ascii += arr[i] + '.';
            else if (this.punycodeUtf.test(arr[i])) {
               // convert from UTF8 to Punycode ASCII
               var tmp = Conetserv.Punycode.encode(arr[i]);
               // check if valid ASCII
               if (this.punycodeAscii.test(tmp))
                  ascii += 'xn--' + tmp + '.';
               else
                  return false
            }
            else
               return false;
         }
         // try to convert TLD
         if (this.punycodeAsciiTld.test(arr[arr.length - 1]))
            ascii += arr[arr.length - 1];
         else if (this.punycodeUtfTld.test(arr[arr.length - 1])) {
            // convert from UTF8 to Punycode ASCII
            var tmp = Conetserv.Punycode.encode(arr[arr.length - 1]);
            // check if valid ASCII
            if (this.punycodeAscii.test(tmp))
               ascii += 'xn--' + tmp;
            else
               return false
         }
         else
            return false;
         this.hostname = ascii;
         // Parse domain from hostname string
         if (this.domainFromHostname.test(this.hostname)){
            this.domain = this.domainFromHostname.exec(this.hostname)[1];
         }
      }

      return true;
   }
};
