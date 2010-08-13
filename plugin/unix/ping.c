#include <stdio.h>

#include "debug.h"
#include "plugin_npapi.h"
#include "plugin_module.h"
#include "init_modules.h"
#include "shell.h"

static module *m = NULL;
static shell_command *cmd = NULL;

static process *
start(const NPVariant *args, const uint32_t argc)
{
   char *argv[10];
   int i = 0;

   if (!cmd || !cmd->found)
      return NULL;

   if (argc < 1)
      return NULL;

   if (args[0].type != NPVariantType_String)
      return NULL;

   argv[i++] = "ping";
   argv[i++] = "-n";

   /*
   while (i < argc) {

   }
   */

   argv[i++] = (char *)STRING_UTF8CHARACTERS(args[0].value.stringValue);
   argv[i++] = NULL;

   return shell->run(cmd->path, argv);
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
destroy()
{
   DEBUG_STR("m->destroy()");

   if (cmd) {
      if (cmd->path != NULL)
         browser->memfree(cmd->path);
      browser->memfree(cmd);
   }

   if (m) {
      if (m->obj)
         browser->releaseobject(m->obj);
      browser->memfree(m);
   }
}

module *
init_module_ping()
{
   DEBUG_STR("%s->init()", "ping");

   /* Module initialization */
   if ((m = browser->memalloc(sizeof(*m))) == NULL)
      goto err_m_alloc;

   m->next = NULL;
   if ((m->obj = browser->createobject(instance, &pluginClass)) == NULL)
      goto err_obj_create;
   m->identifier = browser->getstringidentifier("ping");

   m->destroy = destroy;
   m->start = start;

   m->hasProperty = hasProperty;
   m->getProperty = getProperty;

   /* Shell command initialization */
   if ((cmd = browser->memalloc(sizeof(*cmd))) == NULL)
      goto err_cmd_alloc;

   cmd->path = NULL;
   cmd->found = false;
   if (shell && (cmd->path = shell->find("ping")) != NULL) {
      cmd->found = true;
   }

   return m;

err_cmd_alloc:
   browser->releaseobject(m->obj);

err_obj_create:
   browser->memfree(m);

err_m_alloc:
   return NULL;
}
