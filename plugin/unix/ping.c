#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

module *ping = NULL;
module *ping6 = NULL;

typedef struct _object_ping {
   object       obj;
   shell_module *program;
} object_ping;

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[10];
   int i, j;
   char *ptr;
   shell_module *program;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->pingX->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      if (argc < 1)
         return false;

      program = ((object_ping *)obj)->program;

      i = 0;
      /* First argument must be url */
      if (args[i].type != NPVariantType_String)
         return false;
      argv[i++] = program->path;

      /* Set some arguments automatically */
      argv[i++] = "-n"; /* Numeric output only */
      argv[i++] = "-l3"; /* Preload - Send 3 packets without waiting for reply */

      j = 1;
      /* Parse arguments */
      while (j < argc) {
         if (args[j].type == NPVariantType_String && !strcmp(STRING_UTF8CHARACTERS(args[j].value.stringValue), "-4"))
            program = (shell_module *)ping;

         if (args[j].type == NPVariantType_String && !strcmp(STRING_UTF8CHARACTERS(args[j].value.stringValue), "-6"))
            program = (shell_module *)ping6;

         /* -c count */
         /* -i interval */
         /* -t ttl */
         /* -s packetsize */
         j++;
      }

      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);
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

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->ping->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((object_ping *)obj)->program->found, *result);
      return true;
   }
   DEBUG_STR("plugin->ping->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("ping->destroy()");
   if (ping)
      shell->destroy_module((shell_module *)ping);

   DEBUG_STR("ping6->destroy()");
   if (ping6)
      shell->destroy_module((shell_module *)ping6);
}

static NPObject *
allocate_ping(NPP instance, NPClass *class)
{
   object_ping *obj;

   DEBUG_STR("plugin->ping->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (shell_module *)ping;

   return (NPObject *)obj;
}

static NPObject *
allocate_ping6(NPP instance, NPClass *class)
{
   object_ping *obj;

   DEBUG_STR("plugin->ping6->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (shell_module *)ping6;

   return (NPObject *)obj;
}

bool
init_module_ping()
{
   DEBUG_STR("ping->init()");
   ping = (module *)shell->init_module("ping");
   ping->destroy = destroy;
   ping->class = modules->class;
   ping->class.allocate = allocate_ping;
   ping->class.invoke = invokeMethod;
   ping->class.hasProperty = hasProperty;
   ping->class.getProperty = getProperty;

   DEBUG_STR("ping6->init()");
   ping6 = (module *)shell->init_module("ping6");
   ping6->destroy = destroy;
   ping6->class = modules->class;
   ping6->class.allocate = allocate_ping6;
   ping6->class.invoke = invokeMethod;
   ping6->class.hasProperty = hasProperty;
   ping6->class.getProperty = getProperty;

   return true;
}
