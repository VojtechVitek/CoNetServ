#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

/* DIG module */
module *dig = NULL;

/**
 * Object DIG
 * Used for distinguish DIG object passed from JavaScript
 */
typedef struct _object_dig {
   object       obj;
} object_dig;

/**
 * DIG settings
 */
/*
static struct {
} settings = {0};
*/

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[20];
   int i;
   char *ptr;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->dig->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      /* First argument should be command path/name */
      i = 0;
      argv[i++] = ((shell_module *)dig)->path;

      /* Set the URL as the last argument */
      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);

      /* Set NULL-terminating argument */
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (shell->run((process *)result->value.objectValue, ((shell_module *)dig)->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->dig->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->dig->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->dig->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->dig->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((shell_module *)dig)->found, *value);
      return true;
   }
   DEBUG_STR("plugin->dig->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   DEBUG_STR("plugin->dig->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("dig->destroy()");
   if (dig)
      shell->destroy_module((shell_module *)dig);
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object_dig *obj;

   DEBUG_STR("plugin->dig->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;

   return (NPObject *)obj;
}

bool
init_module_dig()
{
   DEBUG_STR("dig->init()");
   dig = (module *)shell->init_module("dig");
   dig->destroy = destroy;
   dig->class = modules->class;
   dig->class.allocate = allocate;
   dig->class.invoke = invokeMethod;
   dig->class.hasProperty = hasProperty;
   dig->class.getProperty = getProperty;
   dig->class.setProperty = setProperty;

   return true;
}
