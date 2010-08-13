#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_object.h"
#include "plugin_process.h"
#include "plugin_module.h"
#include "shell.h"

/** Plugin processes */
process_list    *processes = NULL;

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("process.invokeDefault()");
   return false;
}

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   /* Processess */
   if (processes) {
      if (identifier == processes->read || identifier == processes->stop)
         DEBUG_STR("process.hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
         return true;
   }

   DEBUG_STR("process.hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));

   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   process *p;

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

   DEBUG_STR("plugin.invoke(): false");

   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   module *m;

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

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   process *p;
   NPString str;
   int len;

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

   return false;
}

static void
destroy(process_list *processes)
{
   process *it, *del;

   DEBUG_STR("processs->destroy()");

   shell->destroy();

   for (it = processes->first; it != NULL; ) {
      del = it;
      it = it->next;
      del->destroy(del);
      browser->memfree(del);
   }

   browser->memfree(processes);
}

process_list *
init_processes()
{
   struct _process_list* processes;

   DEBUG_STR("processes->init()");

   processes = browser->memalloc(sizeof(struct _process_list));
   processes->first = NULL;

   processes->read = browser->getstringidentifier("read");
   processes->stop = browser->getstringidentifier("stop");

   processes->destroy = destroy;

   return processes;
}

NPClass processClass = {
   NP_CLASS_STRUCT_VERSION,
   NULL/*allocate*/,
   NULL/*deallocate*/,
   NULL/*invalidate*/,
   hasMethod,
   invokeMethod,
   invokeDefault,
   hasProperty,
   getProperty,
   NULL/*setProperty*/,
   NULL/*removeProperty*/,
   NULL/*enumerate*/,
   NULL/*construct*/
};
