#pragma once
#ifndef _PLUGIN_MODULE_H_
#define _PLUGIN_MODULE_H_

#include "npapi.h"
#include "plugin.h"
#include "process.h"

/** Plugin module - struct */
typedef struct _module {
   NPClass        class;
   NPIdentifier   *identifier;

   void           (*destroy)();
} module;

bool init_modules();

extern module *modules;

NPClass moduleClass;

#endif /*_PLUGIN_MODULE_H_*/
