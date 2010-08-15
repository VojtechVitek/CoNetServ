#include <stdio.h>

#include "debug.h"
#include "npapi.h"
#include "identifier.h"
#include "init_modules.h"
#include "shell.h"

module *modules = NULL;

static void
destroy()
{
   DEBUG_STR("modules->destroy()");

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
   modules->destroy = destroy;

#ifdef MODULE_PING
   if (!init_module_ping())
      return false;
#endif

   return true;
}
