#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "npapi.h"
#include "module.h"
#include "init_modules.h"
#include "shell.h"

module *ping = NULL;
module *ping6 = NULL;

typedef struct _object_ping {
   object       obj;
   shell_module *program;
} object_ping;

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->start) {
      DEBUG_STR("plugin->pingX->hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }

   DEBUG_STR("plugin->pingX->hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[10];
   int i = 0;
   shell_module *program;

   if (identifier == identifiers->start) {

      if (argc < 1)
         return false;

      if (args[0].type != NPVariantType_String)
         return false;

      argv[i++] = "ping";
      argv[i++] = "-n";

      program = ((object_ping *)obj)->program;
      /*
      while (i < argc) {
         if (argv[i] == "-4")
            program = ping4;

         if (argv[i] == "-6")
            program = ping6;
      }
      */

      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);
      DEBUG_STR("plugin->ping->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      if (shell->run((process *)result->value.objectValue, program->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->pingX->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("plugin->pingX->invokeDefault(): false");
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   DEBUG_STR("plugin->pingX->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   DEBUG_STR("plugin->pingX->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

NPClass class = {
   NP_CLASS_STRUCT_VERSION,
   NULL/*allocate*/, /* Set in init_module_ping() */
   NULL/*deallocate*/,
   NULL/*invalidate*/,
   hasMethod,
   invokeMethod,
   invokeDefault,
   hasProperty,
   getProperty,
   NULL/*setProperty*/,
   NULL/*removeProperty*/,
   NULL/*enumerate*/,
   NULL/*construct*/
};

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
   ping->class = class;
   ping->class.allocate = allocate_ping;

   DEBUG_STR("ping6->init()");
   ping6 = (module *)shell->init_module("ping6");
   ping6->destroy = destroy;
   ping6->class = class;
   ping6->class.allocate = allocate_ping6;

   return true;
}
