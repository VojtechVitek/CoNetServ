#pragma once
#ifndef _MODULES_H_
#define _MODULES_H_

#include "plugin_module.h"

struct _module_list* init_modules();
struct _process_list* init_processes();

module* init_module_ping();

#endif /*_MODULES_H_*/