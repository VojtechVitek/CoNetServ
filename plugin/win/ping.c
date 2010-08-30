#include <stdio.h>

#include "debug.h"
#include "cmd_exe.h"
#include "identifier.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"

module *ping = NULL;
module *ping6 = NULL;

typedef struct _object_ping {
   object       obj;
   cmd_exe_module *program;
} object_ping;

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
      if (((object_ping *)obj)->program == (cmd_exe_module *)ping6)
         ptr = strcpy(ptr, "ping -6 ");
      else
         ptr = strcpy(ptr, "ping ");

      /* Get first argument - the url */
      if (args[i].type != NPVariantType_String)
         return false;

      /* Copy URL to command string */
      /* FIXME: Switch char argv[100] to dynamic-allocated array
                and fix this MAGIC constraint
       */
      if (STRING_UTF8LENGTH(args[i].value.stringValue) > 80)
         return false;
      ptr = memcpy(ptr,
                   (char *)STRING_UTF8CHARACTERS(args[i].value.stringValue),
                   STRING_UTF8LENGTH(args[i].value.stringValue));

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
destroy_ping()
{
   DEBUG_STR("ping->destroy()");
   if (ping)
      cmd_line->destroy_module((cmd_exe_module *)ping);
}

static void
destroy_ping6()
{
   DEBUG_STR("ping6->destroy()");
   if (ping6)
      cmd_line->destroy_module((cmd_exe_module *)ping6);
}

static NPObject *
allocate_ping(NPP instance, NPClass *class)
{
   object_ping *obj;

   DEBUG_STR("plugin->ping->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (cmd_exe_module *)ping;

   return (NPObject *)obj;
}

static NPObject *
allocate_ping6(NPP instance, NPClass *class)
{
   object_ping *obj;

   DEBUG_STR("plugin->ping6->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (cmd_exe_module *)ping6;

   return (NPObject *)obj;
}

bool
init_module_ping()
{
   DEBUG_STR("ping->init()");
   ping = (module *)cmd_line->init_module("ping");
   ping->destroy = destroy_ping;
   ping->class = modules->class;
   ping->class.allocate = allocate_ping;
   ping->class.invoke = invokeMethod;
   ping->class.hasProperty = hasProperty;
   ping->class.getProperty = getProperty;

   return true;
}

bool
init_module_ping6()
{
   DEBUG_STR("ping6->init()");
   ping6 = (module *)cmd_line->init_module("ping6");
   ping6->destroy = destroy_ping6;
   ping6->class = modules->class;
   ping6->class.allocate = allocate_ping6;
   ping6->class.invoke = invokeMethod;
   ping6->class.hasProperty = hasProperty;
   ping6->class.getProperty = getProperty;

   return true;
}
