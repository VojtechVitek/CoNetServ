#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

/*
 * !!!
 * Include common part for all systems.
 * !!!
 */
#include "../common/route.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[20];
   int i;
   char *ptr;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->route->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* No argument can be passed from JavaScript */
      if (argc != 0)
         return false;

      /* First argument should be command path/name */
      i = 0;
      argv[i++] = ((shell_module *)route)->path;

      /* Set NULL-terminating argument */
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (shell->run((process *)result->value.objectValue, ((shell_module *)route)->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->route->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

bool
init_module_route()
{
   DEBUG_STR("route->init()");
   route = (module *)shell->init_module("route");
   route->destroy = destroy;
   route->class = modules->class;
   route->class.allocate = allocate;
   route->class.invoke = invokeMethod;
   route->class.hasProperty = hasProperty;
   route->class.getProperty = getProperty;
   route->class.setProperty = setProperty;

   return true;
}
