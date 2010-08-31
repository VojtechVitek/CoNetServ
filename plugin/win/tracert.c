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
#include "../common/traceroute.c"

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char argv[200];
   char *ptr;
   cmd_exe_module *program;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->tracerouteX->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      ptr = argv; /* position in command string we are creating */

      /* Choose program (traceroute or traceroute6) */
      program = ((object_traceroute *)obj)->program;
      if (((object_traceroute *)obj)->program == (cmd_exe_module *)traceroute6) {
         strcpy(ptr, "tracert -6 ");
         ptr += strlen("tracert -6 ");
      } else {
         strcpy(ptr, "tracert ");
         ptr += strlen("tracert ");
      }

      /* Set user-defined arguments */
      if (settings.maxhops > 0) {
         ptr += _snprintf(ptr, 20, "-h %d ", settings.maxhops);
      }
      if (settings.waittime > 0) {
         ptr += _snprintf(ptr, 20, "-w %d ", settings.waittime);
      }
      if (!settings.iptohostname) {
         ptr += _snprintf(ptr, 20, "-d ");
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

   DEBUG_STR("plugin->tracerouteX->invokeMethod(s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}
