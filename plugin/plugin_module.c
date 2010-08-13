#include <stdio.h>

#include "debug.h"
#include "plugin_npapi.h"
#include "init_modules.h"
#include "shell.h"

/** Plugin modules */
module_list     *modules = NULL;

static bool
invokeDefault(NPObject *obj, const NPVariant *args, const uint32_t argCount, NPVariant *result)
{
   DEBUG_STR("module.invokeDefault()");
   return false;
}

static bool
hasMethod(NPObject *obj, NPIdentifier identifier)
{
   /* Modules */
   if (modules) {
      if (identifier == modules->start) {
         DEBUG_STR("module.hasMethod(%s): true", DEBUG_IDENTIFIER(identifier));
         return true;
      }
   }
}

static bool
invokeMethod(NPObject *obj, NPIdentifier identifier, const NPVariant *args, uint32_t argc, NPVariant *result)
{
   module *m;
   process *p, **l;
   NPString str;

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {

         if (obj == m->obj) {
            if (identifier == modules->start) {

               if ((p = m->start(args, argc)) != NULL) {
                  DEBUG_STR("%s.%s(): process", DEBUG_IDENTIFIER(m->identifier), DEBUG_IDENTIFIER(identifier));
                  p->obj = browser->createobject(instance, &processClass);
                  browser->retainobject(p->obj);
                  OBJECT_TO_NPVARIANT(p->obj, *result);
                  l = &(processes->first);
                  while (*l != NULL)
                     l = &((*l)->next);
                  *l = p;
                  return true;
               } else {
                  DEBUG_STR("%s.%s(): false", DEBUG_IDENTIFIER(m->identifier), DEBUG_IDENTIFIER(identifier));
                  BOOLEAN_TO_NPVARIANT(false, *result);
                  return true;
               }
            } else {
               return m->getProperty(identifier, result);
            }
         }

      }

   }
}

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   module *m;

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {
         if (obj == m->obj)
            return m->hasProperty(identifier);
      }

   }

   DEBUG_STR("module.hasProperty(%s): MODULE NOT AN OBJECT", DEBUG_IDENTIFIER(identifier));

   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *result)
{
   module *m;

   /* Modules */
   if (modules && modules->first) {

      for (m = modules->first; m != NULL; m = m->next) {
         if (obj == m->obj)
            return m->getProperty(identifier, result);
      }

   }

   DEBUG_STR("module.getProperty(%s): MODULE NOT AN OBJECT", DEBUG_IDENTIFIER(identifier));

   return false;
}

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

NPClass moduleClass = {
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
