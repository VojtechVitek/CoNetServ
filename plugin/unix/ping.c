#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

/* Internal PING and PING6 modules */
module *ping = NULL;
module *ping6 = NULL;

/**
 * Object PING - abstraction of PING and PING6
 * Used for distinguish PING and PING6 objects passed from JavaScript
 */
typedef struct _object_ping {
   object       obj;
   shell_module *program;
} object_ping;

/**
 * PING settings
 * The same settings for both PING and PING6
 */
struct {
   int count;
   int interval;
   int packetsize;
   int ttl;
} settings = {0};

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

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->count) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->interval) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->packetsize) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->ttl) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->ping->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((object_ping *)obj)->program->found, *value);
      return true;
   }
   if (identifier == identifiers->count) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.count, *value);
      return true;
   }
   if (identifier == identifiers->interval) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.interval, *value);
      return true;
   }
   if (identifier == identifiers->packetsize) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.packetsize, *value);
      return true;
   }
   if (identifier == identifiers->ttl) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.ttl, *value);
      return true;
   }
   DEBUG_STR("plugin->ping->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   if (identifier == identifiers->count) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.count = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == identifiers->interval) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.interval = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == identifiers->packetsize) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.packetsize = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == identifiers->ttl) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.ttl = NPVARIANT_TO_INT32(*value);
      return true;
   }
   DEBUG_STR("plugin->ping->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy_ping()
{
   DEBUG_STR("ping->destroy()");
   if (ping)
      shell->destroy_module((shell_module *)ping);
}

static void
destroy_ping6()
{
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
   ping->destroy = destroy_ping;
   ping->class = modules->class;
   ping->class.allocate = allocate_ping;
   ping->class.invoke = invokeMethod;
   ping->class.hasProperty = hasProperty;
   ping->class.getProperty = getProperty;
   ping->class.setProperty = setProperty;

   return true;
}

bool
init_module_ping6()
{
   DEBUG_STR("ping6->init()");
   ping6 = (module *)shell->init_module("ping6");
   ping6->destroy = destroy_ping6;
   ping6->class = modules->class;
   ping6->class.allocate = allocate_ping6;
   ping6->class.invoke = invokeMethod;
   ping6->class.hasProperty = hasProperty;
   ping6->class.getProperty = getProperty;
   ping6->class.setProperty = setProperty;

   return true;
}
