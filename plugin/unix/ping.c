#include "debug.h"
#include "init_modules.h"
#include "plugin_npapi.h"
#include "plugin_module.h"
#include "unix_command.h"

char *default_path = "/bin/ping";
char *default_name = "ping";
char *arguments;

static bool
init()
{
   /*
   // vyhledat, zda existuje v systemu
   NPUTF8            *paths[];

   // zkusit spustit
   if (!module->obj) {
      module->obj = npnfuncs->createobject(instance, &npcRefObject);
      npnfuncs->retainobject(plugin);
   }
   module->identifier = NPN_GetStringIdentifier("ping");
   */
}

static void
destroy()
{

}

void init_module_ping() {

}