#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_module.h"
#include "modules.h"

/*! NPAPI variables */
NPObject        *plugin   = NULL;
NPNetscapeFuncs *npnfuncs = NULL;
NPP              instance = NULL;

/*! Plugin variables */
NPIdentifier version;

/** Plugin modules */
module_list  *modules;

static bool
hasMethod(NPObject* obj, NPIdentifier methodName)
{
   debug("hasMethod(%s)", npnfuncs->utf8fromidentifier(methodName));
   return false;
}

static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
   debug("invoke(%s)", npnfuncs->utf8fromidentifier(methodName));
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName)
{
   module *it;

   if (obj == plugin) {
      /* Plugin main object */

      /* Plugin version */
      if (propertyName == version) {
         debug("plugin.hasProperty(version)");
         return true;
      }

      debug("plugin.hasProperty(%s)", npnfuncs->utf8fromidentifier(propertyName));

   } else {
      /* Find module object */
      for (it = modules->first; it != NULL; it = it->next) {
         if (obj == it->obj) {
            /* Return hasProperty of module, if found */
            return it->hasProperty(propertyName);
            debug("plugin.%s.hasProperty(%s)",
                  npnfuncs->utf8fromidentifier(it->identifier),
                  npnfuncs->utf8fromidentifier(propertyName));
         }
      }
   }

   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result)
{
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
         debug("plugin.getProperty(version)");

         return true;
      }

   debug("plugin.getProperty(%s)", npnfuncs->utf8fromidentifier(propertyName));

   } else {
      /* Find module object */
      for (it = modules->first; it != NULL; it = it->next) {
         if (obj == it->obj) {
            /* Return hasProperty of module, if found */
            debug("plugin.%s.getProperty(%s)",
                  npnfuncs->utf8fromidentifier(it->identifier),
                  npnfuncs->utf8fromidentifier(propertyName));
            return it->getProperty(propertyName, result);
         }
      }
   }

   return false;
}

/* NPP */
static NPError
init(NPMIMEType pluginType, NPP _instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved)
{
   instance = _instance;
   debug("init(instance %d, pluginType %d, mode %d, argc %d)", instance, pluginType, mode, argc);
   return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save)
{
   /* Stop possibly running processes. */

   if(plugin)
      npnfuncs->releaseobject(plugin);
   plugin = NULL;
   debug("destroy()");
   return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value)
{
   switch(variable) {
   case NPPVpluginNameString:
      debug("getValue(NPPVPluginNameString)");
      *((char **)value) = PLUGIN_NAME;
      break;
   case NPPVpluginDescriptionString:
      debug("getValue(NPPVPluginDescriptionString)");
      *((char **)value) = PLUGIN_DESC;
      break;
   case NPPVpluginScriptableNPObject:
      debug("getValue(NPPVpluginScriptableNPObject");
      if(!plugin)
         plugin = npnfuncs->createobject(instance, &npclass);
      npnfuncs->retainobject(plugin);
      *(NPObject **)value = plugin;
      break;
   case NPPVpluginNeedsXEmbed:
      debug("getValue(NPPVpluginNeedsXEmbed)");
      *((bool *)value) = true;
      break;
   case NPPVpluginKeepLibraryInMemory:
      debug("getValue(NPPVpluginKeepLibraryInMemory)");
      *((bool *)value) = true;
      break;
   default:
      debug("getValue(default) - ERROR");
      return NPERR_GENERIC_ERROR;
   }
   return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev)
{
   debug("handleEvent()");
   return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow)
{
   debug("setWindow()");
   return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs)
{
   debug("NP_GetEntryPoints()");
   nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
   nppfuncs->newp          = init;
   nppfuncs->destroy       = destroy;
   nppfuncs->getvalue      = getValue;
   nppfuncs->event         = handleEvent;
   nppfuncs->setwindow     = setWindow;

   return NPERR_NO_ERROR;
}

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf
#if defined(ANDROID)
         , NPPluginFuncs *nppfuncs, JNIEnv *env, jobject plugin_object
#elif !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
         , NPPluginFuncs *nppfuncs
#endif
         )
{
   module *it;

   debug("NP_Initialize()");
   if(npnf == NULL)
      return NPERR_INVALID_FUNCTABLE_ERROR;

   if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
      return NPERR_INCOMPATIBLE_VERSION_ERROR;

   npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
   NP_GetEntryPoints(nppfuncs);
#endif

   version = npnfuncs->getstringidentifier("version");

   modules = init_modules();

   return NPERR_NO_ERROR;
}

NPError
OSCALL NP_Shutdown()
{
   debug("NP_Shutdown()");

   modules->destroy();

   return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void)
{
   debug("NP_GetMIMEDescription");
   return PLUGIN_MIME;
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value)
{
   debug("NP_GetValue");
   return getValue((NPP)npp, variable, value);
}

NPClass npclass = {
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

#ifdef __cplusplus
}
#endif
