#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "npapi.h"
#include "module.h"
#include "init_modules.h"
#include "shell.h"

module *ping = NULL;

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->start) {
      DEBUG_STR("ping->hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }

   DEBUG_STR("ping->hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   if (identifier == identifiers->start) {
      DEBUG_STR("ping->getMethod(%s): true", DEBUG_IDENTIFIER(identifier));
      char *argv[10];
      int i = 0;

      if (argc < 1)
         return NULL;

      if (args[0].type != NPVariantType_String)
         return NULL;

      argv[i++] = "ping";
      argv[i++] = "-n";

      /*
      while (i < argc) {

      }
      */

      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);
      if (shell->run((process *)result->value.objectValue, ((shell_module *)ping)->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("ping->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}


static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   DEBUG_STR("ping->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   DEBUG_STR("ping->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("ping->destroy()");

   if (ping) {
      ping->destroy();
   }
}

bool
init_module_ping()
{
   ping = (module *)shell->module("ping");
   ping->class = moduleClass;
   ping->class.hasMethod =   hasMethod;
   ping->class.invoke =      invokeMethod;
   ping->class.hasProperty = hasProperty;
   ping->class.getProperty = getProperty;

   return true;
}
