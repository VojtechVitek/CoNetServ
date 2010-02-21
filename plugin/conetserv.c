/* ***** BEGIN LICENSE BLOCK *****
 * (C)opyright 2008-2009 Aplix Corporation. anselm@aplixcorp.com
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 * ***** END LICENSE BLOCK ***** */

#include <stdio.h>
#include <string.h>

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

#include "conetserv.h"

static NPObject        *so       = NULL;
static NPNetscapeFuncs *npnfuncs = NULL;
static NPP              inst     = NULL;

/* NPN */

static void logmsg(const char *msg) {
#if defined(ANDROID)
   FILE *out = fopen("/tmp/conetserv.log", "a");
	if(out) {
		fputs(msg, out);
		fclose(out);
	}
#elif !defined(_WINDOWS)
	fputs(msg, stderr);
#else
   FILE *out = fopen("\\conetserv.log", "a");
	if(out) {
		fputs(msg, out);
		fclose(out);
	}
#endif
}

static bool
hasMethod(NPObject* obj, NPIdentifier methodName) {
   logmsg("conetserv: hasMethod ");
   logmsg(methodName);
   logmsg("()\n");
	return true;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
   logmsg("conetserv: invokeDefault\n");
   result->type = NPVariantType_Bool;
   result->value.boolValue = true;
   return true;
}

static bool
startPing(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
   logmsg("conetserv: startPing\n");
   result->type = NPVariantType_Bool;
   result->value.boolValue = true;
	return true;
}

static bool
stopPing(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
   logmsg("conetserv: stopPing\n");
   result->type = NPVariantType_Bool;
   result->value.boolValue = true;
   return true;
}

static bool
readPing(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
   logmsg("conetserv: readPing\n");

   const char msg[] = "84.2 ms\n";
   char *txt = (char *)npnfuncs->memalloc(strlen(msg));
   memcpy(txt, msg, strlen(msg));
   NPString str = { txt, strlen(msg) };
   result->type = NPVariantType_String;
   result->value.stringValue = str;

   return true;
}


static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
   logmsg("conetserv: invoke\n");
	int error = 1;
	char *name = npnfuncs->utf8fromidentifier(methodName);
	if(name) {
      if(!strcmp(name, "startPing")) {
         if(argCount == 1 && args[0].type == NPVariantType_String) {
           logmsg("conetserv: startPing(\"string\")\n");
           return startPing(obj, args, argCount, result);
         }
      }
      else if(!strcmp(name, "stopPing")) {
         if(argCount == 0) {
           logmsg("conetserv: stopPing()\n");
           return stopPing(obj, args, argCount, result);
         }
      }
      else if(!strcmp(name, "readPing")) {
         if(argCount == 0) {
           logmsg("conetserv: readPing()\n");
           return readPing(obj, args, argCount, result);
         }
      }
	}
	// aim exception handling
   npnfuncs->setexception(obj, "no such method");
	return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName) {
   logmsg("conetserv: hasProperty\n");
	return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
   logmsg("conetserv: getProperty\n");
	return false;
}

static NPClass npcRefObject = {
	NP_CLASS_STRUCT_VERSION,
	NULL,
	NULL,
	NULL,
	hasMethod,
	invoke,
	invokeDefault,
	hasProperty,
	getProperty,
	NULL,
	NULL,
};

/* NPP */

static NPError
nevv(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved) {
	inst = instance;
   logmsg("conetserv: new\n");
	return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save) {
	if(so)
		npnfuncs->releaseobject(so);
	so = NULL;
   logmsg("conetserv: destroy\n");
	return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value) {
	inst = instance;
	switch(variable) {
	default:
      logmsg("conetserv: getvalue - default\n");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
      logmsg("conetserv: getvalue - name string\n");
      *((char **)value) = PLUGIN_NAME;
		break;
	case NPPVpluginDescriptionString:
      logmsg("conetserv: getvalue - description string\n");
      *((char **)value) = PLUGIN_DESC;
		break;
	case NPPVpluginScriptableNPObject:
      logmsg("conetserv: getvalue - scriptable object\n");
		if(!so)
			so = npnfuncs->createobject(instance, &npcRefObject);
		npnfuncs->retainobject(so);
		*(NPObject **)value = so;
		break;
#if defined(XULRUNNER_SDK)
	case NPPVpluginNeedsXEmbed:
      logmsg("conetserv: getvalue - xembed\n");
      *((bool *)value) = false;
		break;
#endif
	}
	return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev) {
	inst = instance;
   logmsg("conetserv: handleEvent\n");
	return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow) {
	inst = instance;
   logmsg("conetserv: setWindow\n");
	return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
   logmsg("conetserv: NP_GetEntryPoints\n");
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	nppfuncs->newp          = nevv;
	nppfuncs->destroy       = destroy;
	nppfuncs->getvalue      = getValue;
	nppfuncs->event         = handleEvent;
	nppfuncs->setwindow     = setWindow;

	return NPERR_NO_ERROR;
}

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32_t)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf
#if defined(ANDROID)
			, NPPluginFuncs *nppfuncs, JNIEnv *env, jobject plugin_object
#elif !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
			, NPPluginFuncs *nppfuncs
#endif
			)
{
   logmsg("conetserv: NP_Initialize\n");
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
	NP_GetEntryPoints(nppfuncs);
#endif
	return NPERR_NO_ERROR;
}

NPError
OSCALL NP_Shutdown() {
   logmsg("conetserv: NP_Shutdown\n");
	return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void) {
   logmsg("conetserv: NP_GetMIMEDescription\n");
   return PLUGIN_MIME;
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value) {
	inst = (NPP)npp;
	return getValue((NPP)npp, variable, value);
}

#ifdef __cplusplus
}
#endif

