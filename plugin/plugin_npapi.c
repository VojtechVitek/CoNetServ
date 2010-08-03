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
module_list  *modules = NULL;

/** Plugin processes */
process_list *processes = NULL;

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   module *m;
   process *p;

   /* Plugin object */
   if (obj == plugin) {

      DEBUG_STR("plugin.hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
      return false;

   }

   /* Modules */
   if (modules) {
      if (identifier == modules->start) {
         DEBUG_STR("module.hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
         return true;
      }
   }

   /* Processess */
   if (processes) {
      if (identifier == processes->read || identifier == processes->stop)
         DEBUG_STR("process.hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
         return true;
   }

   DEBUG_STR("module/process.hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));

   return false;
}

static bool
invoke(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   module *m;
   process *p, **l;
   NPString str;

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {

         if (obj == m->obj) {
            if (identifier == modules->start) {

               if ((p = m->start(m, args, argc)) != NULL) {
                  DEBUG_STR("%s.%s(): process", DEBUG_IDENTIFIER(m->identifier), DEBUG_IDENTIFIER(identifier));
                  p->obj = npnfuncs->createobject(instance, &npclass);
                  npnfuncs->retainobject(p->obj);
                  OBJECT_TO_NPVARIANT(p->obj, *result);
                  l = &(processes->first);
                  while (*l != NULL)
                     l = &((*l)->next);
                  *l = p;
                  return true;
               } else {
                  DEBUG_STR("%s.%s(): false", DEBUG_IDENTIFIER(m->identifier), DEBUG_IDENTIFIER(identifier));
                  BOOLEAN_TO_NPVARIANT(false, *result);
                  return true;
               }
            } else {
               return m->getProperty(m, identifier, result);
            }
         }

      }

   }

   /* Module objects (running processes) */
   if (processes && processes->first) {

      for (p = processes->first; p != NULL; p = p->next) {

         if (obj == p->obj) {
            if (identifier == processes->read) {
               if (p->read(p, result)) {
                  DEBUG_STR("process.read(): string");
                  return true;
               } else {
                  DEBUG_STR("process.read(): false");
                  BOOLEAN_TO_NPVARIANT(false, *result);
                  return true;
               }
            } else if (identifier == processes->stop) {
               if (p->stop(p)) {
                  DEBUG_STR("process.stop(): true");
                  BOOLEAN_TO_NPVARIANT(true, *result);
               } else {
                  DEBUG_STR("process.stop(): false");
                  BOOLEAN_TO_NPVARIANT(false, *result);
               }
               process *del = p;

               if (processes->first == p) {
                  processes->first = NULL;
               } else {
                  for (p = processes->first; p->next != del; p = p->next);
                  p->next = del->next;
               }
               del->destroy(del);
               return true;
            }
         }

      }
   }

   DEBUG_STR("module/process.invoke(): false");

   return false;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("plugin.invokeDefault()");
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   module *m;
   process *p;

   /* Plugin object */
   if (obj == plugin) {

      /* Plugin version */
      if (identifier == version) {
         DEBUG_STR("plugin.hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));

         return true;
      }

      /* Plugin modules */
      if (modules && modules->first) {

         for (m = modules->first; m != NULL; m = m->next) {
            if (identifier == m->identifier) {
               DEBUG_STR("plugin.hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));

               return true;
            }
         }

      }

      DEBUG_STR("plugin.hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));

      return false;

   }

   DEBUG_STR("module/process.hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));

   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   module *m;
   process *p;
   NPString str;
   int len;

   /* Plugin main object */
   if (obj == plugin) {

      /* Plugin version */
      if (identifier == version) {

         DEBUG_STR("plugin.%s: string", DEBUG_IDENTIFIER(identifier));

         len = strlen(VERSION);
         NPUTF8 *version = npnfuncs->memalloc((len + 1) * sizeof(NPUTF8));
         strcpy(version, VERSION);
         STRING_UTF8CHARACTERS(str) = version;
         STRING_UTF8LENGTH(str) = len;

         result->type = NPVariantType_String;
         result->value.stringValue = str;

         return true;
      }

      /* Plugin modules */
      if (modules && modules->first) {

         for (m = modules->first; m != NULL; m = m->next) {
            if (identifier == m->identifier) {
               DEBUG_STR("plugin.%s: object", DEBUG_IDENTIFIER(identifier));

               OBJECT_TO_NPVARIANT(m->obj, *result);
               npnfuncs->retainobject(m->obj);

               return true;
            }
         }

      }

   }

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {
         if (obj == m->obj)
            return m->getProperty(m, identifier, result);
      }

   }

   return false;
}

/* NPP */
static NPError
init(const NPMIMEType pluginType, const NPP _instance, const uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved)
{
   DEBUG_STR("init(instance %d, pluginType %d, mode %d, argc %d)", instance, pluginType, mode, argc);

   instance = _instance;
   return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save)
{
   /* Stop possibly running processes. */
   DEBUG_STR("destroy()");

   if (plugin)
      npnfuncs->releaseobject(plugin);

   plugin = NULL;
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
      if(!plugin)
         plugin = npnfuncs->createobject(instance, &npclass);
      /* The caller (browser) is responsible to release object (ref_count--).
       * But we need (at least) object ref_count == 1
       */
      npnfuncs->retainobject(plugin);
      *(NPObject **)value = plugin;
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
      DEBUG_STR("getValue(default) - ERROR");
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
   //nppfuncs->size          = sizeof(NPPPluginFuncs);
   nppfuncs->newp          = init;
   nppfuncs->destroy       = destroy;
   nppfuncs->getvalue      = getValue;
   nppfuncs->event         = handleEvent;
   //nppfuncs->setwindow     = setWindow;
/*
   pluginFuncs->version    = (NP_VERSION_MAJOR << 8) + NP_VERSION_MINOR;
   pluginFuncs->size       = sizeof(NPPluginFuncs);
   pluginFuncs->newp       = NewNPP_NewProc(Private_New);
   pluginFuncs->destroy    = NewNPP_DestroyProc(Private_Destroy);
   pluginFuncs->setwindow  = NewNPP_SetWindowProc(Private_SetWindow);
   pluginFuncs->newstream  = NewNPP_NewStreamProc(Private_NewStream);
   pluginFuncs->destroystream = NewNPP_DestroyStreamProc(Private_DestroyStream);
   pluginFuncs->asfile     = NewNPP_StreamAsFileProc(Private_StreamAsFile);
   pluginFuncs->writeready = NewNPP_WriteReadyProc(Private_WriteReady);
   pluginFuncs->write      = NewNPP_WriteProc(Private_Write);
   pluginFuncs->print      = NewNPP_PrintProc(Private_Print);
   pluginFuncs->urlnotify  = NewNPP_URLNotifyProc(Private_URLNotify);
   pluginFuncs->getvalue   = NewNPP_GetValueProc(Private_GetValue);
   pluginFuncs->event      = NewNPP_HandleEventProc(Private_HandleEvent);
#ifdef OJI
        pluginFuncs->javaClass  = Private_GetJavaClass();
#endif*/
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

   DEBUG_STR("NP_Initialize()");
   if (npnf == NULL)
      return NPERR_INVALID_FUNCTABLE_ERROR;

   if (HIBYTE(npnf->version) > NP_VERSION_MAJOR)
      return NPERR_INCOMPATIBLE_VERSION_ERROR;

   npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
   NP_GetEntryPoints(nppfuncs);
#endif

   version = npnfuncs->getstringidentifier("version");

   modules = init_modules();

   processes = init_processes();

   return NPERR_NO_ERROR;
}

NPError OSCALL
NP_Shutdown()
{
   DEBUG_STR("NP_Shutdown()");

   modules->destroy(modules);

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

NPClass npclass = {
   NP_CLASS_STRUCT_VERSION,
   NULL/*allocate*/,
   NULL/*deallocate*/,
   NULL/*invalidate*/,
   hasMethod,
   invoke,
   invokeDefault,
   hasProperty,
   getProperty,
   NULL/*setProperty*/,
   NULL/*removeProperty*/,
   NULL/*enumerate*/,
   NULL/*construct*/
};
