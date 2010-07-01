#pragma once
#ifndef _SHELL_H_
#define _SHELL_H_

#include "plugin_npapi.h"

#define BUFLEN 1024

typedef struct _cmd_shell {
   void (*destroy)();
   char* (*find)(char *program_name);
   void (*run)(char *program_path);
} cmd_shell;

cmd_shell * init_shell();

extern cmd_shell *shell;

#endif /*_SHELL_H_*/
