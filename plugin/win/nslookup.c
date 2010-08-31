#include <stdio.h>

#include "debug.h"
#include "cmd_exe.h"
#include "identifier.h"
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
#include "../common/nslookup.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char argv[200];
   char *ptr;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->nslookup->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      if (argc < 1)
         return false;

      ptr = argv; /* position in command string we are creating */

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      strcpy(ptr, "nslookup ");
      ptr += strlen("nslookup ");

      /* Set user-defined arguments */
      switch (settings.query) {
      case 1:
         ptr += _snprintf(ptr, 20, "-q=AAAA ");
         break;
      case 0:
      default:
         /* nothing .. -q=A is default for nslookup command */
         break;
      }

      /* Set the URL as the last argument */
      if (STRING_UTF8LENGTH(args[0].value.stringValue) > 100)
         return false;
      memcpy(ptr,
             (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue),
             STRING_UTF8LENGTH(args[0].value.stringValue));
      ptr += STRING_UTF8LENGTH(args[0].value.stringValue);

      /* Set NULL-terminating character */
      *ptr = '\0';

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (cmd_line->run((process *)result->value.objectValue, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->nslookup->invokeMethod(s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}
