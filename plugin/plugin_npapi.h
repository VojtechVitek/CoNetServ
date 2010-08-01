#pragma once
#ifndef _PLUGIN_NPAPI_H_
#define _PLUGIN_NPAPI_H_

#if defined(_WINDOWS)
#include "stdbool.h"
#define _X86_
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

/* Return high byte of the variable */
#ifndef HIBYTE
#define HIBYTE(x) ((((uint32_t)(x)) & 0xff00) >> 8)
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

/*! NPAPI variables */
extern NPObject        *plugin;
extern NPNetscapeFuncs *npnfuncs;
extern NPP              instance;
extern NPClass          npclass;

/*! EXPORT functions */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs);

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf
#if defined(ANDROID)
         , NPPluginFuncs *nppfuncs, JNIEnv *env, jobject plugin_object
#elif !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
         , NPPluginFuncs *nppfuncs
#endif
         );

NPError OSCALL
NP_Shutdown();

NPError OSCALL
NP_GetValue(void *npp, NPPVariable variable, void *value);

#ifdef __cplusplus
}
#endif

#endif /*_PLUGIN_NPAPI_H_*/
