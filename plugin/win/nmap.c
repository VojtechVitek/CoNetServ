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
#include "../common/nmap.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char argv[200];
   char *ptr;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->nmap->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      ptr = argv; /* position in command string we are creating */

      strcpy(ptr, "nmap ");
      ptr += strlen("nmap ");

      /* Set user-defined arguments */
      switch (settings.query) {
      case 1: /* scan local neighboars */
         ptr += _snprintf(ptr, 20, "-sT ");
         break;
      case 0: /* scan ports of a host */
      default:
         /* nothing .. default for nmap command */
         break;
      }

      /* FIXME: */
      /* Test if there is ':' char -> IPv6 */
      tmp = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);
      while (*tmp++ != '\0') {
         if (*tmp == ':') {
            memcpy(ptr, "-6", 3);
            ptr += 3;
            break;
         }
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

   DEBUG_STR("plugin->nmap->invokeMethod(s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}
