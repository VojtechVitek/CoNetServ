#include <stdio.h>

#include "debug.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

/*
 * !!!
 * Include common part for all systems.
 * !!!
 */
#include "../common/ipconfig.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[20];
   int i;
   char *ptr;

   if (identifier == browser->getstringidentifier("start")) {

      DEBUG_STR("plugin->ipconfig->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* No argument can be passed from JavaScript */
      if (argc != 0)
         return false;

      /* First argument should be command path/name */
      i = 0;
      argv[i++] = ((shell_module *)ipconfig)->path;

      /* Set NULL-terminating argument */
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (shell->run((process *)result->value.objectValue, ((shell_module *)ipconfig)->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->ipconfig->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

bool
init_module_ipconfig()
{
   DEBUG_STR("ipconfig->init()");
   ipconfig = (module *)shell->init_module("ifconfig");
   ipconfig->destroy = destroy;
   ipconfig->class = modules->class;
   ipconfig->class.allocate = allocate;
   ipconfig->class.invoke = invokeMethod;
   ipconfig->class.hasProperty = hasProperty;
   ipconfig->class.getProperty = getProperty;
   ipconfig->class.setProperty = setProperty;

   return true;
}
