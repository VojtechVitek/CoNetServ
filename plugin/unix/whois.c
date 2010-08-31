#include <stdio.h>

#include "debug.h"
#include "identifier.h"
#include "init_modules.h"
#include "module.h"
#include "npapi.h"
#include "shell.h"

/* WHOIS module */
module *whois = NULL;

/**
 * Object WHOIS
 * Used for distinguish WHOIS object passed from JavaScript
 */
typedef struct _object_whois {
   object       obj;
} object_whois;

/**
 * WHOIS settings
 */
static struct {
} settings = {0};

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   char *argv[20];
   int i;
   char *ptr;

   if (identifier == identifiers->start) {

      DEBUG_STR("plugin->whois->invokeMethod(%s): true", DEBUG_IDENTIFIER(identifier));

      /* Just the URL argument can be passed from JavaScript */
      if (argc != 1 || args[0].type != NPVariantType_String)
         return false;

      /* First argument must be url */
      i = 0;
      argv[i++] = ((shell_module *)whois)->path;

      /* Set the URL as the last argument */
      argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);

      /* Set NULL-terminating argument */
      argv[i++] = NULL;

      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &processClass), *result);

      if (shell->run((process *)result->value.objectValue, ((shell_module *)whois)->path, argv))
         return true;
      else
         return false;
   }

   DEBUG_STR("plugin->whois->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->whois->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->whois->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->whois->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((shell_module *)whois)->found, *value);
      return true;
   }
   DEBUG_STR("plugin->whois->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   DEBUG_STR("plugin->whois->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("whois->destroy()");
   if (whois)
      shell->destroy_module((shell_module *)whois);
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object_whois *obj;

   DEBUG_STR("plugin->whois->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;

   return (NPObject *)obj;
}

bool
init_module_whois()
{
   DEBUG_STR("whois->init()");
   whois = (module *)shell->init_module("whois");
   whois->destroy = destroy;
   whois->class = modules->class;
   whois->class.allocate = allocate;
   whois->class.invoke = invokeMethod;
   whois->class.hasProperty = hasProperty;
   whois->class.getProperty = getProperty;
   whois->class.setProperty = setProperty;

   return true;
}
