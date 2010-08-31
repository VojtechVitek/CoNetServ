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
   char argv[200];
   char *ptr;
   cmd_exe_module *program;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->pingX->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

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

      /* Set user-defined arguments */
      if (settings.count > 0) {
         ptr += _snprintf(ptr, 20, "-n %d ", settings.count);
      } else if (settings.count == 0) {
         ptr += _snprintf(ptr, 20, "-t ");
      }
      if (settings.packetsize > 0) {
         ptr += _snprintf(ptr, 20, "-l %d ", settings.packetsize);
      }
      if (settings.timeout > 0) {
         ptr += _snprintf(ptr, 20, "-w %d ", settings.timeout);
      }
      if (settings.ttl > 0) {
         ptr += _snprintf(ptr, 20, "-i %d ", settings.ttl);
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

   DEBUG_STR("plugin->pingX->invokeMethod(s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}
