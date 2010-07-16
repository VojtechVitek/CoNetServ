#include <stdio.h>

#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_module.h"
#include "modules.h"
#include "shell.h"

static bool
start()
{

}

static void
read(NPUTF8 *result)
{

}

static bool
stop()
{

}

static bool
hasProperty(NPIdentifier propertyName)
{
   return false;
}

static bool
getProperty(NPIdentifier propertyName, NPVariant *result)
{
   return false;
}

static void
destroy(module *it)
{
   DEBUG_STR("ping->destroy()");
   npnfuncs->memfree(it);
   npnfuncs->releaseobject(it->obj);
}

module *
init_module_ping()
{
   module *it;

   DEBUG_STR("ping->init()");

   it = (module *)npnfuncs->memalloc(sizeof(module));
   it->next = NULL;
   it->obj = npnfuncs->createobject(instance, &npclass);
   it->identifier = npnfuncs->getstringidentifier("ping");
   it->program = "ping";
   it->found = false;
   if (shell) {
      if ((it->path = shell->find("ping")) != NULL) {
         it->found = true;
      }
   }
   it->destroy = destroy;
   it->start = start;
   it->read = read;
   it->stop = stop;
   it->hasProperty = hasProperty;
   it->getProperty = getProperty;

   return it;
}
