#include <stdio.h>

#include "debug.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

static bool
invokeMethod(NPObject *, NPIdentifier, const NPVariant *, uint32_t, NPVariant *);

/*
 * !!!
 * Include common part for all systems.
 * !!!
 */
#include "../common/nmap.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[20];
   char query[20];
   int i;
   char *ptr;

   if (identifier == browser->getstringidentifier("start")) {

      DEBUG_STR("plugin->nmap->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      /* First argument should be command path/name */
      i = 0;
      argv[i++] = ((shell_module *)nmap)->path;

      /* Set user-defined arguments */
      switch (settings.query) {
      case 1: /* scan local neighboars */
         if (snprintf(query, 20, "-sT"))
            argv[i++] = query;
         break;
      case 0: /* scan ports of a host */
      default:
         /* nothing .. default for nmap command */
         break;
      }

      /* Set the URL as the last argument */
      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);

      /* Set NULL-terminating argument */
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (shell->run((process *)result->value.objectValue, ((shell_module *)nmap)->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->nmap->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}
