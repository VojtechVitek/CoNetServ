#pragma once
#ifndef _SHELL_H_
#define _SHELL_H_

#include "plugin_npapi.h"
#include "plugin_module.h"

#define BUFLEN 1024

typedef struct _shell_command {
   NPUTF8         *path;
   bool           found;
   int            err;
} shell_command;

typedef struct _cmd_shell {
   void (*destroy)();
   char * (*find)(char *program_name);
   process * (*run)(const char *path, char *const argv[]);
} cmd_shell;

cmd_shell *init_shell();

extern cmd_shell *shell;

#endif /*_SHELL_H_*/
