#include <stdio.h>

#include "debug.h"
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
   char *argv[20];
   char count[20], interval[20], packetsize[20], ttl[20];
   int i;
   char *ptr;
   shell_module *program;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->pingX->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      program = ((object_ping *)obj)->program;

      /* First argument must be url */
      i = 0;
      argv[i++] = program->path;

      /* Set default arguments */
      argv[i++] = "-n"; /* Numeric output only */
      argv[i++] = "-l3"; /* Preload - Send 3 packets without waiting for reply */

      /* Set user-defined arguments */
      if (settings.count > 0) {
         if (snprintf(count, 20, "-c %d", settings.count))
            argv[i++] = count;
      }
      if (settings.interval > 0) {
         if (snprintf(interval, 20, "-i %d", settings.interval))
            argv[i++] = interval;
      }
      if (settings.packetsize > 0) {
         if (snprintf(packetsize, 20, "-s %d", settings.packetsize))
            argv[i++] = packetsize;
      }
      if (settings.ttl > 0) {
         if (snprintf(ttl, 20, "-t %d", settings.ttl))
            argv[i++] = ttl;
      }

      /* Set the URL as the last argument */
      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);

      /* Set NULL-terminating argument */
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (shell->run((process *)result->value.objectValue, program->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->pingX->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}