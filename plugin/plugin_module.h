#pragma once
#ifndef _PLUGIN_MODULE_H_
#define _PLUGIN_MODULE_H_

#include "plugin_npapi.h"
#include "plugin_object.h"
#include "plugin_process.h"

/** Plugin module - struct */
typedef struct _module {
   struct _module *next;

   NPObject       *obj;
   NPIdentifier   *identifier;

   void           (*destroy)(struct _module *m);
   process        *(*start)(const NPVariant *args, const uint32_t argc);
   bool           (*hasProperty)(const NPIdentifier propertyName);
   bool           (*getProperty)(const NPIdentifier propertyName, NPVariant *result);

} module;

/** Plugin module list - struct */
typedef struct _module_list {
   struct _module *first;

   NPIdentifier    start;

   void            (*destroy)();

} module_list;

/** Plugin modules */
extern module_list *modules;
extern NPClass     moduleClass;

#endif /*_PLUGIN_MODULE_H_*/
