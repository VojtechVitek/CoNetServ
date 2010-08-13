#pragma once
#ifndef _INIT_MODULES_H_
#define _INIT_MODULES_H_

#include "plugin_module.h"

struct _module_list* init_modules();
struct _process_list* init_processes();

module* init_module_ping();

#define platform_init_modules(it)   \
   do {                             \
      *it = init_module_ping();     \
      it = &((*it)->next);          \
   } while (0)


#endif /*_INIT_MODULES_H_*/