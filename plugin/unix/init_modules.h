#pragma once
#ifndef _INIT_MODULES_H_
#define _INIT_MODULES_H_

#include "module.h"
#include "shell.h"

#define MODULE_PING
module *ping, *ping6;
bool init_module_ping();

#endif /*_INIT_MODULES_H_*/