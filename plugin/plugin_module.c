#include <stdio.h>

#include "debug.h"
#include "plugin_npapi.h"
#include "init_modules.h"
#include "shell.h"

static void
destroy()
{
   module *it, *del;

   DEBUG_STR("modules->destroy()");

   shell->destroy();

   for (it = modules->first; it != NULL; ) {
      del = it;
      it = it->next;
      del->destroy(del);
      browser->memfree(del);
   }

   browser->memfree(modules);
}

module_list *
init_modules()
{
   struct _module_list* modules;
   module **it;

   DEBUG_STR("modules->init()");

   shell = init_shell();

   modules = browser->memalloc(sizeof(struct _module_list));
   modules->first = NULL;
   it = &(modules->first);

   modules->start = browser->getstringidentifier("start");

   modules->destroy = destroy;

   /* Platform-specific module initialization */
   /* (Macro using iterator it) */
   platform_init_modules(it);

   return modules;
}

static void
destroy_processes(process_list *processes)
{
   process *it, *del;

   DEBUG_STR("processs->destroy()");

   shell->destroy();

   for (it = processes->first; it != NULL; ) {
      del = it;
      it = it->next;
      del->destroy(del);
      browser->memfree(del);
   }

   browser->memfree(processes);
}

process_list *
init_processes()
{
   struct _process_list* processes;

   DEBUG_STR("processes->init()");

   processes = browser->memalloc(sizeof(struct _process_list));
   processes->first = NULL;

   processes->read = browser->getstringidentifier("read");
   processes->stop = browser->getstringidentifier("stop");

   processes->destroy = destroy_processes;

   return processes;
}