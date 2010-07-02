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

}

static bool
getProperty(NPIdentifier propertyName, NPVariant *result)
{

}

static void
destroy(module *it)
{
   DEBUG_STR("ping->destroy()");
   npnfuncs->memfree(it);
   npnfuncs->releaseobject(it->obj);
}

module *
init_module_ping() {
   module *it;

   DEBUG_STR("init_module_ping()");
   it = (module *)npnfuncs->memalloc(sizeof(module));
   it->obj = npnfuncs->createobject(instance, &npclass);
   it->identifier = npnfuncs->getstringidentifier("ping");
   it->path = "ping";

   it->next = NULL;
   it->destroy = destroy;
   it->start = start;
   it->read = read;
   it->stop = stop;
   it->hasProperty = hasProperty;
   it->getProperty = getProperty;

   return it;
}
