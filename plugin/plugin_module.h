#pragma once
#ifndef _PLUGIN_MODULE_H_
#define _PLUGIN_MODULE_H_

#include "plugin_npapi.h"

/** Plugin module - struct */
typedef struct _module {
   struct _module    *next;
   NPObject          *obj;
   NPIdentifier      *identifier;

   /*
   NPUTF8            *version;
   NPUTF8
   */

   int               not_found;
   int               err;

   bool (*init)();
   void (*destroy)();
   bool (*start)();
   void (*read)(NPUTF8 *result;);
   bool (*stop)();
   bool (*hasProperty)(NPIdentifier propertyName);
   bool (*getProperty)(NPIdentifier propertyName, NPVariant *result);

} module;

/** Plugin modules */
extern module        *modules;

#endif /*_PLUGIN_MODULE_H_*/
