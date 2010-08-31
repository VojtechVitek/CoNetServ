#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "identifier.h"
#include "init_modules.h"
#include "npapi.h"
#include "plugin.h"
#include "module.h"

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   DEBUG_STR("plugin->hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   DEBUG_STR("plugin->invokeMethod(): false");
   return false;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("plugin->invokeDefault(): false");
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   /* Plugin version */
   if (identifier == identifiers->version) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }

   /* Plugin modules */
#ifdef MODULE_PING
   if (identifier == identifiers->ping) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_PING6
   if (identifier == identifiers->ping6) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_TRACEROUTE
   if (identifier == identifiers->traceroute || identifier == identifiers->tracert) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_TRACEROUTE6
   if (identifier == identifiers->traceroute6 || identifier == identifiers->tracert6) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_NSLOOKUP
   if (identifier == identifiers->nslookup) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_WHOIS
   if (identifier == identifiers->whois) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_DIG
   if (identifier == identifiers->dig) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

#ifdef MODULE_NMAP
   if (identifier == identifiers->nmap) {
      DEBUG_STR("plugin->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
#endif

   DEBUG_STR("plugin->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   int len;
   NPString str;
   NPUTF8 *version;

   /* Plugin version */
   if (identifier == identifiers->version) {

      len = strlen(VERSION);
      version = browser->memalloc((len + 1) * sizeof(*version));
      strcpy(version, VERSION);
      STRING_UTF8CHARACTERS(str) = version;
      STRING_UTF8LENGTH(str) = len;

      DEBUG_STR("plugin->getProperty(%s): %s", DEBUG_IDENTIFIER(identifier), version);


      /* msvc can't handle simple macro
       * STRINGN_TO_NPVARIANT(version, len, *result);
       */

      result->type = NPVariantType_String;
      result->value.stringValue = str;

      return true;
   }

   /* Plugin modules */
#ifdef MODULE_PING
   if (identifier == identifiers->ping) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(ping->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_PING6
   if (identifier == identifiers->ping6) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(ping6->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_TRACEROUTE
   if (identifier == identifiers->traceroute || identifier == identifiers->tracert) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(traceroute->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_TRACEROUTE6
   if (identifier == identifiers->traceroute6 || identifier == identifiers->tracert6) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(traceroute6->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_NSLOOKUP
   if (identifier == identifiers->nslookup) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(nslookup->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_WHOIS
   if (identifier == identifiers->whois) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(whois->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_DIG
   if (identifier == identifiers->dig) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(dig->class)), *result);
      return true;
   }
#endif

#ifdef MODULE_NMAP
   if (identifier == identifiers->nmap) {
      DEBUG_STR("plugin->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      OBJECT_TO_NPVARIANT(browser->createobject(((object *)obj)->instance, &(nmap->class)), *result);
      return true;
   }
#endif

   DEBUG_STR("plugin->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));

   return false;
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object *obj;

   DEBUG_STR("plugin->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   obj->instance = instance;

   return (NPObject *)obj;
}

static void
deallocate(NPObject *obj)
{
   DEBUG_STR("plugin->deallocate()");
   browser->memfree(obj);
}

NPClass pluginClass = {
   NP_CLASS_STRUCT_VERSION,
   allocate,
   deallocate,
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
