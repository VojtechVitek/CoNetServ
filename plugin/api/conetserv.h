/*!
 * \brief CoNetServ object
 *
 * CoNetServ plugin should be loaded into (X)HTML document using object tag:
 * <object id="conetserv" type="application/x-conetserv" width="0" height="0">
 *   Plugin FAILED to load
 * </object>
 *
 * Getting JavaScript variable from object tag:
 * var conetserv = document.getElementById("conetserv");
 *
 */
class conetserv {

   /*!
    * \brief Plugin version
    */
   string version = "2.0.0";

   /*!
    * \brief PING command
    */
   class ping {

      /*!
       * \brief Start ping command
       * @return bool True if success, false otherwise.
       */
      bool start() {
         /* Native code */
      };

      /*!
       * \brief Stop ping command
       * @return bool True if success, false otherwise.
       */
      bool stop() {
         /* Native code */
      };

      /*!
       * \breaf Read ping command output
       * @return string Data output.
       */
      string read() {
         /* Native code */
      };

   };

   /*!
    * \brief PING6 comand
    */
   class ping6 {

      /*!
       * \brief Start ping6 command
       * @return bool True if success, false otherwise.
       */
      bool start() {
         /* Native code */
      };

      /*!
       * \brief Stop ping6 command
       * @return bool True if success, false otherwise.
       */
      bool stop() {
         /* Native code */
      };

      /*!
       * \breaf Read ping6 command output
       * @return string Data output.
       */
      string read() {
         /* Native code */
      };

   };

   /**
    * \brief TRACEROUTE command
    */
   class traceroute {

      /*!
       * \brief Start traceroute command
       * @return bool True if success, false otherwise.
       */
      bool start() {
         /* Native code */
      };

      /*!
       * \brief Stop traceroute command
       * @return bool True if success, false otherwise.
       */
      bool stop() {
         /* Native code */
      };

      /*!
       * \breaf Read traceroute command output
       * @return string Data output.
       */
      string read() {
         /* Native code */
      };

   };

   /**
    * \brief TRACEROUTE6 command
    */
   class traceroute6 {

      /*!
       * \brief Start traceroute6 command
       * @return bool True if success, false otherwise.
       */
      bool start() {
         /* Native code */
      };

      /*!
       * \brief Stop traceroute6 command
       * @return bool True if success, false otherwise.
       */
      bool stop() {
         /* Native code */
      };

      /*!
       * \breaf Read traceroute6 command output
       * @return string Data output.
       */
      string read() {
         /* Native code */
      };

   };

   /**
    * \brief WHOIS command
    */
   class whois {

      /*!
       * \brief Start whois command
       * @return bool True if success, false otherwise.
       */
      bool start() {
         /* Native code */
      };

      /*!
       * \brief Stop whois command
       * @return bool True if success, false otherwise.
       */
      bool stop() {
         /* Native code */
      };

      /*!
       * \breaf Read whois command output
       * @return string Data output.
       */
      string read() {
         /* Native code */
      };

   };

   /**
    * \brief NSLOOKUP command
    */
   class nslookup {

      /*!
       * \brief Start nslookup command
       * @return bool True if success, false otherwise.
       */
      bool start() {
         /* Native code */
      };

      /*!
       * \brief Stop nslookup command
       * @return bool True if success, false otherwise.
       */
      bool stop() {
         /* Native code */
      };

      /*!
       * \breaf Read nslookup command output
       * @return string Data output.
       */
      string read() {
         /* Native code */
      };

   };

};
