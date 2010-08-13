#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_object.h"
#include "plugin_module.h"

/*! Plugin variables */
NPObject        *plugin   = NULL;
NPP              instance = NULL;
NPIdentifier     version;

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("plugin.invokeDefault(): false");
   return false;
}

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   DEBUG_STR("plugin.hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   DEBUG_STR("plugin.invoke(): false");
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   module *m;

   /* Plugin version */
   if (identifier == version) {
      DEBUG_STR("plugin.hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));

      return true;
   }

   /* Plugin modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {
         if (identifier == m->identifier) {
            DEBUG_STR("plugin.hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));

            return true;
         }
      }

   }

   DEBUG_STR("plugin.hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));

   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   int len;
   NPString str;
   module *m;

   /* Plugin version */
   if (identifier == version) {

      DEBUG_STR("plugin.%s: string", DEBUG_IDENTIFIER(identifier));

      len = strlen(VERSION);
      NPUTF8 *version = browser->memalloc((len + 1) * sizeof(NPUTF8));
      strcpy(version, VERSION);
      STRING_UTF8CHARACTERS(str) = version;
      STRING_UTF8LENGTH(str) = len;

      result->type = NPVariantType_String;
      result->value.stringValue = str;

      return true;
   }

   /* Plugin modules */
   if (modules && modules->first) {
      for (m = modules->first; m != NULL; m = m->next) {
         if (identifier == m->identifier) {

            DEBUG_STR("plugin.%s: object", DEBUG_IDENTIFIER(identifier));

            browser->retainobject(m->obj);
            OBJECT_TO_NPVARIANT(m->obj, *result);
            return true;
         }
      }
   }

   DEBUG_STR("plugin.%s: false", DEBUG_IDENTIFIER(identifier));

   return false;
}

NPClass pluginClass = {
   NP_CLASS_STRUCT_VERSION,
   NULL/*allocate*/,
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
