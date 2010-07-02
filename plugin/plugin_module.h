#pragma once
#ifndef _PLUGIN_MODULE_H_
#define _PLUGIN_MODULE_H_

#include "plugin_npapi.h"

/** Plugin module - struct */
typedef struct _module {
   struct _module *next;
   NPObject       *obj;
   NPIdentifier   *identifier;
   NPUTF8         *program;
   NPUTF8         *path;
   NPUTF8         *version;

   bool           found;
   int            err;

   void (*destroy)(struct _module *);
   bool (*start)();
   void (*read)(NPUTF8 *result);
   bool (*stop)();
   bool (*hasProperty)(NPIdentifier propertyName);
   bool (*getProperty)(NPIdentifier propertyName, NPVariant *result);

} module;

typedef struct _module_list {
   struct _module *first;
   void (*destroy)(struct _module_list *);
} module_list;

/** Plugin modules */
extern module_list *modules;

#endif /*_PLUGIN_MODULE_H_*/
