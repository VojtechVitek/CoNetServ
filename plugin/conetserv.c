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

/* Conforming to 0001-Mozilla-SDK-libxul-1.9.1-support.patch by kwizart*/
#if (((NP_VERSION_MAJOR << 8) + NP_VERSION_MINOR) < 20)
#include "npupp.h"
#else
#include "npfunctions.h"
#endif

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
   logmsg("npsimple: hasMethod ");
   logmsg(methodName);
   logmsg("()\n");
	return true;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	logmsg("npsimple: invokeDefault\n");
	result->type = NPVariantType_Int32;
	result->value.intValue = 42;
	return true;
}

static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	logmsg("npsimple: invoke\n");
	int error = 1;
	char *name = npnfuncs->utf8fromidentifier(methodName);
	if(name) {
		if(!strcmp(name, "foo")) {
			logmsg("npsimple: invoke foo\n");
			return invokeDefault(obj, args, argCount, result);
		}
		else if(!strcmp(name, "callback")) {
			if(argCount == 1 && args[0].type == NPVariantType_Object) {
				static NPVariant v, r;
				const char kHello[] = "Hello";
				char *txt = (char *)npnfuncs->memalloc(strlen(kHello));

				logmsg("npsimple: invoke callback function\n");
				memcpy(txt, kHello, strlen(kHello));
				STRINGN_TO_NPVARIANT(txt, strlen(kHello), v);
				/* INT32_TO_NPVARIANT(42, v); */
				if(npnfuncs->invokeDefault(inst, NPVARIANT_TO_OBJECT(args[0]), &v, 1, &r))
					return invokeDefault(obj, args, argCount, result);
			}
         else {
            logmsg("v-teq: prvni string\n");
            const char msg[] = "TEST ;-)";
            NPString str;
            str.utf8characters = NPN_UTF8FromIdentifier(NPN_GetStringIdentifier(msg));
            str.utf8length = strlen(msg);
            result->type = NPVariantType_String;
            result->value.stringValue = str;
            return true;
         }
		}
	}
	// aim exception handling
	npnfuncs->setexception(obj, "exception during invocation");
	return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName) {
	logmsg("npsimple: hasProperty\n");
	return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
	logmsg("npsimple: getProperty\n");
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
	logmsg("npsimple: new\n");
	return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save) {
	if(so)
		npnfuncs->releaseobject(so);
	so = NULL;
	logmsg("npsimple: destroy\n");
	return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value) {
	inst = instance;
	switch(variable) {
	default:
		logmsg("npsimple: getvalue - default\n");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		logmsg("npsimple: getvalue - name string\n");
      *((char **)value) = "CoNetServ";
		break;
	case NPPVpluginDescriptionString:
		logmsg("npsimple: getvalue - description string\n");
      *((char **)value) = "<a href=\"http://www.fres-solutions.com/conetserv/\">CoNetServ</a> - Complex Network Services plugin.";
		break;
	case NPPVpluginScriptableNPObject:
      logmsg("npsimple: getvalue - scriptable object");
		if(!so)
			so = npnfuncs->createobject(instance, &npcRefObject);
		npnfuncs->retainobject(so);
		*(NPObject **)value = so;
		break;
#if defined(XULRUNNER_SDK)
	case NPPVpluginNeedsXEmbed:
		logmsg("npsimple: getvalue - xembed\n");
      *((bool *)value) = false;
		break;
#endif
	}
	return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev) {
	inst = instance;
	logmsg("npsimple: handleEvent\n");
	return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow) {
	inst = instance;
	logmsg("npsimple: setWindow\n");
	return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
	logmsg("npsimple: NP_GetEntryPoints\n");
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
	logmsg("npsimple: NP_Initialize\n");
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
	logmsg("npsimple: NP_Shutdown\n");
	return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void) {
	logmsg("npsimple: NP_GetMIMEDescription\n");
   return "application/x-conetserv::Complex Network Services";
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value) {
	inst = (NPP)npp;
	return getValue((NPP)npp, variable, value);
}

#ifdef __cplusplus
}
#endif

