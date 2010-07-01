#include <stdio.h>

#include "debug.h"
#include "plugin_npapi.h"
#include "modules.h"
#include "shell.h"

static void
destroy()
{
   module *it, *del;

   shell->destroy();

   for (it = modules->first; it != NULL; ) {
      del = it;
      it = it->next;
      del->destroy();
      npnfuncs->memfree(del);
   }

   npnfuncs->memfree(modules);
}

struct _module_list *
init_modules()
{
   struct _module_list* modules;
   module **it;

   debug("init_modules()");

   shell = init_shell();

   modules = npnfuncs->memalloc(sizeof(struct _module_list));
   modules->first = NULL;
   it = &(modules->first);
   modules->destroy = destroy;

   /* Initialize modules */
   *it = init_module_ping();
   it = &((*it)->next);

   return modules;
}
