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
   if (identifier == identifiers->read) {
      DEBUG_STR("process->hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }

   if (identifier == identifiers->stop) {
      DEBUG_STR("process->hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }

   DEBUG_STR("process->hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   /* Read data from process */
   if (identifier == identifiers->read) {

      DEBUG_STR("process->invokeMethod(read)")

      if (!shell->read((process *)obj, result)) {
         DEBUG_STR("process->invokeMethod(read): false");
         BOOLEAN_TO_NPVARIANT(false, *result);
      }

      return true;
   }

   /* Stop/kill the process */
   if (identifier == identifiers->stop) {

      DEBUG_STR("process->invokeMethod(stop)");

      if (shell->stop((process *)obj)) {
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
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("process->invokeDefault(): false");
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
   p->running = false;

   return (NPObject *)p;
}

static void
deallocate(NPObject *obj)
{
   DEBUG_STR("process->deallocate()");

   /* Process is still running */
   if (((process *)obj)->running)
      shell->stop((process *)obj);

   browser->memfree(obj);
}

NPClass processClass = {
   NP_CLASS_STRUCT_VERSION,
   allocate,
   deallocate,
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
