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
#include "../common/ping.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char argv[100];
   int i;
   char *ptr;
   cmd_exe_module *program;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->pingX->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      if (argc < 1)
         return false;

      i = 0; /* position in arguments passed from JavaScript */
      ptr = argv; /* position in command string we are creating */

      /* Choose program (ping or ping6) */
      program = ((object_ping *)obj)->program;
      if (((object_ping *)obj)->program == (cmd_exe_module *)ping6) {
         strcpy(ptr, "ping -6 ");
         ptr += strlen("ping -6 ");
      } else {
         strcpy(ptr, "ping ");
         ptr += strlen("ping ");
      }

      /* Get first argument - the url */
      if (args[i].type != NPVariantType_String)
         return false;

      /* Copy URL to command string */
      /* FIXME: Switch char argv[100] to dynamic-allocated array
                and fix this MAGIC constraint
       */
      if (STRING_UTF8LENGTH(args[i].value.stringValue) > 80)
         return false;
      memcpy(ptr,
             (char *)STRING_UTF8CHARACTERS(args[i].value.stringValue),
             STRING_UTF8LENGTH(args[i].value.stringValue));
      ptr += STRING_UTF8LENGTH(args[i].value.stringValue);

      *ptr = '\0';

      /* PARSE ARGUMENTS HERE */

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (cmd_line->run((process *)result->value.objectValue, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->pingX->invokeMethod(s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}
