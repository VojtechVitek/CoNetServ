#include <stdio.h>

#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_module.h"
#include "modules.h"
#include "shell.h"

static process *
start(const module *ping, const NPVariant *args, const uint32_t argc)
{
   int i;

   if (!ping->found)
      return NULL;

   /*
   for (i = 0; i < argc; ++i) {

   }
   */

   char * argv[2] = {ping->path, "-n"};

   return shell->run(argv);
}

static bool
hasProperty(const NPIdentifier property)
{
   return false;
}

static bool
getProperty(const NPIdentifier property, NPVariant *result)
{
   return false;
}

static void
destroy(module *ping)
{
   DEBUG_STR("ping->destroy()");
   npnfuncs->memfree(ping);
   npnfuncs->releaseobject(ping->obj);
}

module *
init_module_ping()
{
   module *ping;

   DEBUG_STR("ping->init()");

   if ((ping = (module *)npnfuncs->memalloc(sizeof(module))) == NULL)
      return NULL;

   ping->next = NULL;
   ping->obj = npnfuncs->createobject(instance, &npclass);
   ping->identifier = npnfuncs->getstringidentifier("ping");
   ping->found = false;
   if (shell) {
      if ((ping->path = shell->find("ping")) != NULL) {
         ping->found = true;
      }
   }
   ping->destroy = destroy;
   ping->start = start;

   ping->hasProperty = hasProperty;
   ping->getProperty = getProperty;

   return ping;
}
