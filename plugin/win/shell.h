#pragma once
#ifndef _SHELL_H_
#define _SHELL_H_

/*
 * Windows shell is CMD.EXE.
 * Adapt it's naming convention to an abstract struct SHELL.
 */
#define init_cmd_exe init_shell
#define cmd_exe_module shell_module
#define cmd_exe sshell

#include "cmd_exe.h"

#endif /*_SHELL_H_*/