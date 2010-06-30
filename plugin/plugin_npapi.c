#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_module.h"
#include "init_modules.h"

/*! NPAPI variables */
NPObject        *plugin   = NULL;
NPNetscapeFuncs *npnfuncs = NULL;
NPP              instance = NULL;

/*! Plugin variables */
NPIdentifier version;

static bool
hasMethod(NPObject* obj, NPIdentifier methodName) {
   debug("hasMethod");
   debug(npnfuncs->utf8fromidentifier(methodName));
   debug("()\n");
   return true;
}

static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {

   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName) {
   module *it;

   if (obj == plugin) {
      /* Plugin main object */

      /* Plugin version */
      if (propertyName == version) {
         debug("plugin hasProperty(version)\n");
         return true;
      }

      debug("plugin hasProperty\n");

   } else {
      /* Find module object */
      for (it = modules; it != NULL; it = it->next) {
         if (obj == it->obj) {
            /* Return hasProperty of module, if found */
            return it->hasProperty(propertyName);
         }
      }
   }

   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
   module *it;
#ifdef _WINDOWS
   NPString npstr;
#endif

   if (obj == plugin) {
      /* Plugin main object */

      /* Plugin version */
      if (propertyName == version) {
#ifdef _WINDOWS
         result->type = NPVariantType_String;
         STRING_UTF8CHARACTERS(npstr) = VERSION;
         STRING_UTF8LENGTH(npstr) = strlen(VERSION);
         result->value.stringValue = npstr;
#else
         STRINGN_TO_NPVARIANT(VERSION, strlen(VERSION), *result);
#endif
         debug("plugin getProperty(version)\n");

         return true;
      }

      debug("plugin getProperty\n");

   } else {
      /* Find module object */
      for (it = modules; it != NULL; it = it->next) {
         if (obj == it->obj) {
            /* Return hasProperty of module, if found */
            debug("plugin module getProperty");
            return it->getProperty(propertyName, result);
         }
      }
   }

   return false;
}

static NPClass npcRefObject = {
   NP_CLASS_STRUCT_VERSION,
   NULL/*allocate*/,
   NULL/*deallocate*/,
   NULL/*invalidate*/,
   hasMethod,
   invoke,
   NULL/*invokeDefault*/,
   hasProperty,
   getProperty,
   NULL/*setProperty*/,
   NULL/*removeProperty*/,
   NULL/*enumerate*/,
   NULL/*construct*/
};

/* NPP */
static NPError
init(NPMIMEType pluginType, NPP _instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved) {
   instance = _instance;
   debug("init\n");
   fprintf(stderr, "instance %d, pluginType %d, mode %d\n", instance, pluginType, mode);
   return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save) {
   /* Stop possibly running processes. */

   if(plugin)
      npnfuncs->releaseobject(plugin);
   plugin = NULL;
   debug("destroy\n");
   return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value) {
   switch(variable) {
   default:
      debug("getvalue - default\n");
      return NPERR_GENERIC_ERROR;
   case NPPVpluginNameString:
      debug("getvalue - name string\n");
      *((char **)value) = PLUGIN_NAME;
      break;
   case NPPVpluginDescriptionString:
      debug("getvalue - description string\n");
      *((char **)value) = PLUGIN_DESC;
      break;
   case NPPVpluginScriptableNPObject:
      debug("getvalue - scriptable object\n");
      if(!plugin)
         plugin = npnfuncs->createobject(instance, &npcRefObject);
      //FIXME: Should be still in if block?
      npnfuncs->retainobject(plugin);
      *(NPObject **)value = plugin;
      break;
   case NPPVpluginNeedsXEmbed:
      debug("getvalue - xembed\n");
      *((bool *)value) = true;
      break;
   case NPPVpluginKeepLibraryInMemory:
      debug("getvalue - NPPVpluginKeepLibraryInMemory\n");
      *((bool *)value) = true;
      break;
   }
   return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev) {
   debug("handleEvent\n");
   return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow) {
   debug("setWindow\n");
   return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
   debug("NP_GetEntryPoints\n");
   nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
   nppfuncs->newp          = init;
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
   debug("NP_Initialize\n");
   if(npnf == NULL)
      return NPERR_INVALID_FUNCTABLE_ERROR;

   if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
      return NPERR_INCOMPATIBLE_VERSION_ERROR;

   npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
   NP_GetEntryPoints(nppfuncs);
#endif

   version = npnfuncs->getstringidentifier("version");

   INIT_MODULES();

   return NPERR_NO_ERROR;
}

NPError
OSCALL NP_Shutdown() {
   debug("NP_Shutdown\n");
   return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void) {
   debug("NP_GetMIMEDescription\n");
   return PLUGIN_MIME;
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value) {
   debug("NP_GetValue\n");
   return getValue((NPP)npp, variable, value);
}

#ifdef __cplusplus
}
#endif
