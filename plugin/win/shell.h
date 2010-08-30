#pragma once
#ifndef _SHELL_H_
#define _SHELL_H_

/*
 * Windows shell is CMD.EXE.
 * Adapt it's naming convention to an abstract struct SHELL.
 */
#include "cmd_exe.h"

#define init_shell init_cmd_line
#define shell_module cmd_exe_module

#define shell cmd_line

#endif /*_SHELL_H_*/