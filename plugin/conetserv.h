#ifndef _CONETSERV_H_
#define _CONETSERV_H_

#if defined(_WINDOWS)
	#include <stdbool.h>
#endif

#if defined(XULRUNNER_SDK)

#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>

#elif defined(ANDROID)

#undef HAVE_LONG_LONG
#include <jni.h>
#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>
#define OSCALL
#define NPP_WRITE_TYPE (NPP_WriteProcPtr)
#define NPStringText UTF8Characters
#define NPStringLen  UTF8Length
extern JNIEnv *pluginJniEnv;

#elif defined(WEBKIT_DARWIN_SDK)

#include <Webkit/npapi.h>
#include <WebKit/npfunctions.h>
#include <WebKit/npruntime.h>

#define OSCALL

#elif defined(WEBKIT_WINMOBILE_SDK) /* WebKit SDK on Windows */

#ifndef PLATFORM
#define PLATFORM(x) defined(x)
#endif
#include <npfunctions.h>
#ifndef OSCALL
#define OSCALL WINAPI
#endif

#endif

/* WebKIT and XULRunner differs in struct _NPString member names */
#if defined(WEBKIT_DARWIN_SDK)
#define STRING_UTF8CHARACTERS(_v) ((_v).UTF8Characters)
#define STRING_UTF8LENGTH(_v)     ((_v).UTF8Length)
#else
#define STRING_UTF8CHARACTERS(_v) ((_v).utf8characters)
#define STRING_UTF8LENGTH(_v)     ((_v).utf8length)
#endif

/** NPAPI NPObject variable */
extern NPObject        *so;

/** NPAPI NPNetscapeFuncs variable */
extern NPNetscapeFuncs *npnfuncs;

/** NPAPI NPP variable */
extern NPP              inst;

/** Buffer for reading from pipes */
#define BUFFER_LENGTH 1024

/** System-specific debug function */
void logmsg(const char *msg);

/**
 * Commands callable in NPAPI plugin
 * System called commands
 * Library implemented commands
 */
typedef enum {
   /* system commands: */
   PING = 0,
   PING6,
   TRACEROUTE,
   TRACEROUTE6,
   WHOIS,
	NSLOOKUP,

   command_t_count

   /* implemented commands: */
} command_t;

/**
 * Start command
 * @arg cmd Command number.
 * @arg addr URL / IPv4 / IPv6 address.
 * @return True if successfuly started, false otherwise.
 */
bool startCommand(command_t cmd, NPUTF8* arg_host);

/**
 * Read command
 * @arg cmd Command number.
 * @arg buf Buffer to store read data in.
 * @return Data length, or -1 while error.
 */
int readCommand(command_t cmd, NPUTF8* buf);

/**
 * Stop command
 * @arg cmd Command number.
 * @return True if successfuly stopped, false otherwise.
 */
bool stopCommand(command_t cmd);

#define PLUGIN_NAME "CoNetServ"

#define PLUGIN_LONGNAME "Complex Network Services"

#ifndef VERSION
#error Version must be specified by -DVERSION option
#else
#define PLUGIN_VERSION VERSION
#endif

#define PLUGIN_COPYRIGHT "(c) 2010 FRES-Solutions"

#define PLUGIN_DESC "<a href=\"http://www.fres-solutions.com/conetserv/plugin/?version=" \
                    PLUGIN_VERSION "\">CoNetServ</a> " PLUGIN_VERSION " " \
                    PLUGIN_COPYRIGHT " - " PLUGIN_LONGNAME " plugin."

#define PLUGIN_MIME "application/x-conetserv:conetserv:Complex Network Services"

#endif /*_CONETSERV_H_*/

/*! \mainpage CoNetServ - Complex Network Services
 *
 * \section about About
 *
 * \htmlinclude ../../extension/about.html
 */

