#pragma once
#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#if defined(_WINDOWS)
	#include "stdbool.h"
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
#define STRING_UTF8CHARACTERS(_v) (_v).UTF8Characters
#define STRING_UTF8LENGTH(_v)     (_v).UTF8Length
#elif defined(_WINDOWS)
#define STRING_UTF8CHARACTERS(_v) (_v).UTF8Characters
#define STRING_UTF8LENGTH(_v)     (_v).UTF8Length
#else
#define STRING_UTF8CHARACTERS(_v) (_v).utf8characters
#define STRING_UTF8LENGTH(_v)     (_v).utf8length
#endif

typedef struct _command {
   NPObject          *obj;
   struct _command   *next;

   int               not_found;
   int               err;

   void (*init)();
   void (*start)();
   void (*read)();
   void (*stop)();

   NPIdentifier      *name;
   NPUTF8            *paths[];
};

extern command       *commands;

/** NPAPI NPObject variable */
extern NPObject      *plugin;

/** NPAPI NPNetscapeFuncs variable */
extern NPNetscapeFuncs *npnfuncs;

/** NPAPI NPP variable */
extern NPP              inst;

/** Buffer for reading from pipes */
#define BUFFER_LENGTH 1024

/** System-specific debug function */
void logmsg(const char *msg);

#endif /*_PLUGIN_H_*/

