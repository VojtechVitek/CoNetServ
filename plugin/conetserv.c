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

#include "conetserv.h"

NPObject        *so       = NULL;
NPNetscapeFuncs *npnfuncs = NULL;
NPP              inst     = NULL;

char buffer[BUFFER_LENGTH];

void logmsg(const char *msg) {
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
   logmsg("CoNetServ: hasMethod ");
   logmsg(methodName);
   logmsg("()\n");
	return true;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
   logmsg("CoNetServ: invokeDefault\n");
   result->type = NPVariantType_Bool;
   result->value.boolValue = true;
   return true;
}

static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	NPString str;
   char *name = npnfuncs->utf8fromidentifier(methodName);
	logmsg("CoNetServ: invoke\n");
   if (name) {
      if (!strncmp(name, "read", 4) && argCount == 0) {
         if (!strcmp(name + 4, "Ping")) {
            logmsg("CoNetServ: invoke readPing()\n");
            int len = readCommand(PING, buffer);
            char *txt = (char *)npnfuncs->memalloc(len);
            memcpy(txt, buffer, len);
            str.utf8characters = txt;
            str.utf8length = len;
            result->type = NPVariantType_String;
            result->value.stringValue = str;
            return true;
         }
      } else if (!strncmp(name, "start", 5) && argCount == 1 && args[0].type == NPVariantType_String) {
         if (!strcmp(name + 5, "Ping")) {
           logmsg("CoNetServ: invoke startPing(\"string\")\n");
           result->type = NPVariantType_Bool;
           result->value.boolValue = startCommand(PING, (char*)args[0].value.stringValue.utf8characters);
           return true;
         }
      } else if (!strncmp(name, "stop", 4) && argCount == 0) {
         if(!strcmp(name + 4, "Ping")) {
            logmsg("CoNetServ: invoke stopPing()\n");
            result->type = NPVariantType_Bool;
            result->value.boolValue = stopCommand(PING);
            return true;
         }
      }
	}
   npnfuncs->setexception(obj, "No such method, see CoNetServ programmer's manual.");
	return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName) {
   logmsg("CoNetServ: hasProperty\n");
	return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
   logmsg("CoNetServ: getProperty\n");
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
   logmsg("CoNetServ: new\n");
	return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save) {
   /* Stop possibly running processes. */
   stopCommand(PING);
   stopCommand(TRACEROUTE);
   stopCommand(WHOIS);

	if(so)
		npnfuncs->releaseobject(so);
	so = NULL;
   logmsg("CoNetServ: destroy\n");
	return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value) {
	inst = instance;
	switch(variable) {
	default:
      logmsg("CoNetServ: getvalue - default\n");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
      logmsg("CoNetServ: getvalue - name string\n");
      *((char **)value) = PLUGIN_NAME;
		break;
	case NPPVpluginDescriptionString:
      logmsg("CoNetServ: getvalue - description string\n");
      *((char **)value) = PLUGIN_DESC;
		break;
	case NPPVpluginScriptableNPObject:
      logmsg("CoNetServ: getvalue - scriptable object\n");
		if(!so)
			so = npnfuncs->createobject(instance, &npcRefObject);
		npnfuncs->retainobject(so);
		*(NPObject **)value = so;
		break;
	case NPPVpluginNeedsXEmbed:
      logmsg("CoNetServ: getvalue - xembed\n");
      *((bool *)value) = true;
		break;
	}
	return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev) {
	inst = instance;
   logmsg("CoNetServ: handleEvent\n");
	return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow) {
	inst = instance;
   logmsg("CoNetServ: setWindow\n");
	return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
   logmsg("CoNetServ: NP_GetEntryPoints\n");
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
   logmsg("CoNetServ: NP_Initialize\n");
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
   logmsg("CoNetServ: NP_Shutdown\n");
	return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void) {
   logmsg("CoNetServ: NP_GetMIMEDescription\n");
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

