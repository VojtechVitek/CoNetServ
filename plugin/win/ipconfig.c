#include <stdio.h>

#include "debug.h"
#include "cmd_exe.h"
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
#include "../common/ipconfig.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char argv[200];
   char *ptr;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->ipconfig->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* No argument can be passed from JavaScript */
      if (argc != 0)
         return false;

      ptr = argv; /* position in command string we are creating */

      strcpy(ptr, "ipconfig /All ");
      ptr += strlen("ipconfig /All ");

      /* Set NULL-terminating character */
      *ptr = '\0';

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (cmd_line->run((process *)result->value.objectValue, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->ipconfig->invokeMethod(s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

bool
init_module_ipconfig()
{
   DEBUG_STR("ipconfig->init()");
   ipconfig = (module *)shell->init_module("ipconfig");
   ipconfig->destroy = destroy;
   ipconfig->class = modules->class;
   ipconfig->class.allocate = allocate;
   ipconfig->class.invoke = invokeMethod;
   ipconfig->class.hasProperty = hasProperty;
   ipconfig->class.getProperty = getProperty;
   ipconfig->class.setProperty = setProperty;

   return true;
}
