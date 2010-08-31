#include <stdio.h>

#include "debug.h"
#include "npapi.h"
#include "identifier.h"
#include "init_modules.h"
#include "shell.h"

module *modules = NULL;

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->start) {
      DEBUG_STR("plugin->module->hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }

   DEBUG_STR("plugin->module->hasMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   DEBUG_STR("plugin->module->invokeMethod(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("plugin->module->invokeDefault(): false");
   return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   DEBUG_STR("plugin->module->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   DEBUG_STR("plugin->module->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

NPClass class = {
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

static void
destroy()
{
   DEBUG_STR("modules->destroy()");

#ifdef MODULE_IPCONFIG
   if (ipconfig)
      ipconfig->destroy();
#endif

#ifdef MODULE_NMAP
   if (nmap)
      nmap->destroy();
#endif

#ifdef MODULE_DIG
   if (dig)
      dig->destroy();
#endif

#ifdef MODULE_WHOIS
   if (whois)
      whois->destroy();
#endif

#ifdef MODULE_NSLOOKUP
   if (nslookup)
      nslookup->destroy();
#endif

#ifdef MODULE_TRACEROUTE6
   if (traceroute6)
      traceroute6->destroy();
#endif

#ifdef MODULE_TRACEROUTE
   if (traceroute)
      traceroute->destroy();
#endif

#ifdef MODULE_PING6
   if (ping6)
      ping6->destroy();
#endif

#ifdef MODULE_PING
   if (ping)
      ping->destroy();
#endif

   if (modules)
      browser->memfree(modules);

   if (shell)
      shell->destroy();
}

bool
init_modules()
{
   DEBUG_STR("modules->init()");

   if (!init_shell())
      return false;

   modules = browser->memalloc(sizeof(*modules));
   if (!modules)
      return false;

   modules->class = class;
   modules->destroy = destroy;

#ifdef MODULE_PING
   if (!init_module_ping())
      return false;
#endif

#ifdef MODULE_PING6
   if (!init_module_ping6())
      return false;
#endif

#ifdef MODULE_TRACEROUTE
   if (!init_module_traceroute())
      return false;
#endif

#ifdef MODULE_TRACEROUTE6
   if (!init_module_traceroute6())
      return false;
#endif

#ifdef MODULE_NSLOOKUP
   if (!init_module_nslookup())
      return false;
#endif

#ifdef MODULE_WHOIS
   if (!init_module_whois())
      return false;
#endif

#ifdef MODULE_DIG
   if (!init_module_dig())
      return false;
#endif

#ifdef MODULE_NMAP
   if (!init_module_nmap())
      return false;
#endif

#ifdef MODULE_IPCONFIG
   if (!init_module_ipconfig())
      return false;
#endif

   return true;
}
