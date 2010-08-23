#pragma once
#ifndef _INIT_MODULES_H_
#define _INIT_MODULES_H_

#include "module.h"
#include "shell.h"

#define MODULE_PING
#define MODULE_PING6
module *ping, *ping6;
bool init_module_ping();
#define init_module_ping6() (true) /*ping6 inits in init_module_ping() as well*/

#endif /*_INIT_MODULES_H_*/