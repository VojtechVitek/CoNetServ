#pragma once
#ifndef _PLUGIN_MODULE_H_
#define _PLUGIN_MODULE_H_

#include <unistd.h>

#include "plugin_npapi.h"

/** Plugin process - struct */
typedef struct _process {
   struct _process *next;

   NPObject        *obj;
   NPP             instance;

   pid_t           pid;
   int             pipe[2];

   bool            running;

   void (*destroy)(struct _process *p);
   void (*read)(struct _process *p, NPVariant *result);
   void (*stop)(struct _process *p);

} process;

/** Plugin processes - list */
typedef struct _process_list {
   struct _process *first;

   void (*destroy)(struct _process_list *l);

} process_list;

/** Plugin processes */
extern process_list *processes;


/** Plugin module - struct */
typedef struct _module {
   struct _module *next;

   NPObject       *obj;
   NPIdentifier   *identifier;
   NPUTF8         *path;
   NPUTF8         *version;

   bool           found;
   int            err;

   void (*destroy)(struct _module *m);
   process * (*start)(const struct _module *m, const NPVariant *args, const uint32_t argc);
   bool (*hasProperty)(const NPIdentifier propertyName);
   bool (*getProperty)(const NPIdentifier propertyName, NPVariant *result);

} module;

/** Plugin module list - struct */
typedef struct _module_list {
   struct _module *first;

   void (*destroy)(struct _module_list *l);

} module_list;

/** Plugin modules */
extern module_list *modules;

#endif /*_PLUGIN_MODULE_H_*/
