#pragma once
#ifndef _INIT_MODULES_H_
#define _INIT_MODULES_H_

#include "module.h"
#include "shell.h"

#define MODULE_PING
#define MODULE_PING6
module *ping, *ping6;
bool init_module_ping();
bool init_module_ping6();

#endif /*_INIT_MODULES_H_*/