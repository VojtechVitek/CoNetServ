/**
 * CoNetServ plugin should be loaded into (X)HTML document using object tag:
 * <object id="conetserv" type="application/x-conetserv" width="0" height="0">
 *   Plugin FAILED to load
 * </object>
 *
 * To access the embedded object you can use this JavaScript code:
 * var conetserv = document.getElementById("conetserv");
 *
 * Now, you can access it's methods. For example:
 * conetserv.startPing();
 */
object conetserv;

/**
 * Read output of PING6 command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readPing();

/**
 * Read output of PING6 command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readPing6();

/**
 * Read output of TRACEROUTE command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readTraceroute();

/**
 * Read output of TRACEROUTE6 command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readTraceroute6();

/**
 * Read output of WHOIS command
 * \return String data if successful (could be zero length),
 *         false otherwise (false indicates not yet started or stopped process)
 */
function readWhois();

/**
 * Start PING command
 */
function startPing();

/**
 * Start PING6 command
 */
function startPing6();

/**
 * Start TRACEROUTE command
 */
function startTraceroute();

/**
 * Start TRACEROUTE6 command
 */
function startTraceroute6();

/**
 * Start WHOIS command
 */
function startWhois();


/**
 * Stop PING command
 */
function stopPing();

/**
 * Stop PING6 command
 */
function stopPing6();

/**
 * Stop TRACEROUTE command
 */
function stopTraceroute();

/**
 * Stop TRACEROUTE6 command
 */
function stopTraceroute6();

/**
 * Stop WHOIS command
 */
function stopWhois();
