#ifndef _CONETSERV_H_
#define _CONETSERV_H_

#if !defined(_WINDOWS)
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

extern NPObject        *so;
extern NPNetscapeFuncs *npnfuncs;
extern NPP              inst;

#define BUFFER_LENGTH 1024

void logmsg(const char *msg);

typedef enum {
   /* system commands: */
   PING = 0,
   TRACEROUTE,
   WHOIS,

   command_t_count

   /* implemented commands: */
} command_t;

bool startCommand(command_t cmd, char* addr);
int readCommand(command_t cmd, char* buf);
bool stopCommand(command_t cmd);

#define PLUGIN_NAME "CoNetServ"

#define PLUGIN_LONGNAME "Complex Network Services"

#define PLUGIN_VERSION "1.0"

#define PLUGIN_COPYRIGHT "(c) 2010 FRES-Solutions"

#define PLUGIN_DESC "<a href=\"http://www.fres-solutions.com/conetserv/plugin/?version=" \
                    PLUGIN_VERSION "\">CoNetServ</a> " PLUGIN_VERSION " " \
                    PLUGIN_COPYRIGHT " - " PLUGIN_LONGNAME " plugin."

#define PLUGIN_MIME "application/x-conetserv::Complex Network Services"

#endif /*_CONETSERV_H_*/
