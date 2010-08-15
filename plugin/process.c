#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "identifier.h"
#include "npapi.h"
#include "plugin.h"
#include "process.h"
#include "module.h"
#include "shell.h"

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->read)
      return true;

   if (identifier == identifiers->stop)
      return true;

   DEBUG_STR("process->hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   /* Read data from process */
   if (identifier == identifiers->read) {

      if (shell->read((process *)obj, result)) {
         DEBUG_STR("process->invokeMethod(read): string");
      } else {
         DEBUG_STR("process->invokeMethod(read): false");
         BOOLEAN_TO_NPVARIANT(false, *result);
      }

      return true;
   }

   /* Stop/kill the process */
   if (identifier == identifiers->stop) {

      if (shell->stop((process *)obj)) {
         DEBUG_STR("process->invokeMethod(stop): true");
         BOOLEAN_TO_NPVARIANT(true, *result);
      } else {
         DEBUG_STR("process->invokeMethod(stop): false");
         BOOLEAN_TO_NPVARIANT(false, *result);
      }

      return true;
   }

   DEBUG_STR("process->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->running) {
      DEBUG_STR("process->hasProperty(running): %s", ((process *)obj)->running ? "true" : "false");
      return true;
   }

   DEBUG_STR("process->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   if (identifier == identifiers->running) {
      DEBUG_STR("process->getProperty(running): %s", ((process *)obj)->running ? "true" : "false");
      BOOLEAN_TO_NPVARIANT(((process *)obj)->running, *result);
      return true;
   }

   DEBUG_STR("process->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   process *p;

   DEBUG_STR("process->allocate()");

   p = browser->memalloc(sizeof(*p));
   //p->instance = inst;
   p->running = false;

   return (NPObject *)p;
}

static void
deallocate(NPObject *obj)
{
   /* Process is still running */
   if (((process *)obj)->running)
      shell->stop((process *)obj);

   DEBUG_STR("process->deallocate()");

   browser->memfree(obj);
}

NPClass processClass = {
   NP_CLASS_STRUCT_VERSION,
   allocate,
   deallocate,
   NULL/*invalidate*/,
   hasMethod,
   invokeMethod,
   NULL/*invokeDefault*/,
   hasProperty,
   getProperty,
   NULL/*setProperty*/,
   NULL/*removeProperty*/,
   NULL/*enumerate*/,
   NULL/*construct*/
};
