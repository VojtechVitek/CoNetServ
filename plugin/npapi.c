#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "npapi.h"
#include "plugin.h"
#include "module.h"
#include "init_modules.h"

/*! NPAPI variables */
NPNetscapeFuncs *browser = NULL;

static NPError
init(const NPMIMEType pluginType, const NPP _instance, const uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved)
{
   return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save)
{
   return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, const NPPVariable variable, void *value)
{
   switch (variable) {
   case NPPVpluginNameString:
      DEBUG_STR("getValue(NPPVPluginNameString)");
      *((char **)value) = PLUGIN_NAME;
      break;
   case NPPVpluginDescriptionString:
      DEBUG_STR("getValue(NPPVPluginDescriptionString)");
      *((char **)value) = PLUGIN_DESC;
      break;
   case NPPVpluginScriptableNPObject:
      DEBUG_STR("getValue(NPPVpluginScriptableNPObject)");
      *(NPObject **)value = browser->createobject(instance, &pluginClass);
      break;
   case NPPVpluginNeedsXEmbed:
      DEBUG_STR("getValue(NPPVpluginNeedsXEmbed)");
      /* This plugin use XEmbed instead of using the old Xt-based mainloop */
      *((bool *)value) = true;
      break;
   case NPPVpluginKeepLibraryInMemory:
      DEBUG_STR("getValue(NPPVpluginKeepLibraryInMemory)");
      *((bool *)value) = true;
      break;
   default:
      DEBUG_STR("getValue(%d) - ERROR", variable);
      return NPERR_GENERIC_ERROR;
   }
   return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev)
{
   DEBUG_STR("handleEvent()");
   return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow)
{
   DEBUG_STR("setWindow()");
   return NPERR_NO_ERROR;
}

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs)
{
   DEBUG_STR("NP_GetEntryPoints()");
   nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
   nppfuncs->size          = sizeof(*nppfuncs);
   nppfuncs->newp          = init;
   nppfuncs->destroy       = destroy;
   nppfuncs->getvalue      = getValue;
   //nppfuncs->event         = handleEvent;
   //nppfuncs->setwindow     = setWindow;
   //pluginFuncs->newstream  = NewNPP_NewStreamProc(Private_NewStream);
   //pluginFuncs->destroystream = NewNPP_DestroyStreamProc(Private_DestroyStream);
   //pluginFuncs->asfile     = NewNPP_StreamAsFileProc(Private_StreamAsFile);
   //pluginFuncs->writeready = NewNPP_WriteReadyProc(Private_WriteReady);
   //pluginFuncs->write      = NewNPP_WriteProc(Private_Write);
   //pluginFuncs->print      = NewNPP_PrintProc(Private_Print);
   //pluginFuncs->urlnotify  = NewNPP_URLNotifyProc(Private_URLNotify);
   //pluginFuncs->getvalue   = NewNPP_GetValueProc(Private_GetValue);
   //pluginFuncs->event      = NewNPP_HandleEventProc(Private_HandleEvent);
#ifdef OJI
   //pluginFuncs->javaClass  = Private_GetJavaClass();
#endif
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
   DEBUG_STR("NP_Initialize()");

   if (npnf == NULL)
      return NPERR_INVALID_FUNCTABLE_ERROR;

   if (HIBYTE(npnf->version) > NP_VERSION_MAJOR)
      return NPERR_INCOMPATIBLE_VERSION_ERROR;

   browser = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
   NP_GetEntryPoints(nppfuncs);
#endif

   if (!init_modules())
      return NPERR_MODULE_LOAD_FAILED_ERROR;

   return NPERR_NO_ERROR;
}

NPError OSCALL
NP_Shutdown()
{
   DEBUG_STR("NP_Shutdown()");

   if (modules)
      modules->destroy();

   return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription()
{
   DEBUG_STR("NP_GetMIMEDescription()");
   return PLUGIN_MIME;
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value)
{
   DEBUG_STR("NP_GetValue()");
   return getValue((NPP)npp, variable, value);
}
