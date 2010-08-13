#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_object.h"
#include "plugin_module.h"

/*! Plugin variables */
NPObject        *plugin   = NULL;
NPP              instance = NULL;
NPIdentifier     version;

/** Plugin modules */
module_list     *modules = NULL;

/** Plugin processes */
process_list    *processes = NULL;


static bool hasMethod(NPObject *obj, NPIdentifier identifier)
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

static bool invoke(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   module *m;
   process *p, **l;
   NPString str;

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {

         if (obj == m->obj) {
            if (identifier == modules->start) {

               if ((p = m->start(args, argc)) != NULL) {
                  DEBUG_STR("%s.%s(): process", DEBUG_IDENTIFIER(m->identifier), DEBUG_IDENTIFIER(identifier));
                  p->obj = browser->createobject(instance, &pluginClass);
                  browser->retainobject(p->obj);
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
               return m->getProperty(identifier, result);
            }
         }

      }

   }

   /* Module objects (running processes) */
   if (processes && processes->first) {

      for (p = processes->first; p != NULL; p = p->next) {

         if (obj == p->obj) {
            if (identifier == processes->read) {
               /* Read data from process */
               if (p->read(p, result)) {
                  DEBUG_STR("process.read(): string");
                  return true;
               } else {
                  DEBUG_STR("process.read(): false");
                  BOOLEAN_TO_NPVARIANT(false, *result);
                  return true;
               }
            } else if (identifier == processes->stop) {
               /* Stop/kill the process */
               if (p->stop(p)) {
                  DEBUG_STR("process.stop(): true");
                  BOOLEAN_TO_NPVARIANT(true, *result);
               } else {
                  DEBUG_STR("process.stop(): false");
                  BOOLEAN_TO_NPVARIANT(false, *result);
               }
               /* Process to be deleted */
               process *del = p;

               /* Delete process from process list */
               p = processes->first;
               if (del == processes->first) {
                  /* Process to delete is on the 1st position */
                  processes->first = p->next;
               } else {
                  /* Process to delete is on the 2nd,3rd,.. position */
                  while (p->next != del)
                     p = p->next;
                  p->next = del->next;
               }

               /* Destroy the process */
               del->destroy(del);

               return true;
            }
         }

      }
   }

   DEBUG_STR("module/process.invoke(): false");

   return false;
}

static bool invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("plugin.invokeDefault()");
   return false;
}

static bool hasProperty(NPObject *obj, NPIdentifier identifier)
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

static bool getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
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
         NPUTF8 *version = browser->memalloc((len + 1) * sizeof(NPUTF8));
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
               browser->retainobject(m->obj);

               return true;
            }
         }

      }

   }

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {
         if (obj == m->obj)
            return m->getProperty(identifier, result);
      }

   }

   return false;
}

NPClass pluginClass = {
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
