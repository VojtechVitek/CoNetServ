#pragma once
#ifndef _SHELL_H_
#define _SHELL_H_

#include "npapi.h"
#include "module.h"
#include "process.h"

#define BUFLEN 1024

typedef struct _shell_module {
   module       m;
   bool         found;
   char         *path;
} shell_module;

typedef struct _cmd_shell {
   char         *(*find)(const char *program);
   shell_module *(*init_module)(const char* name);
   void         (*destroy_module)(shell_module *m);
   bool         (*run)(process *p, const char *path, char *const argv[]);
   bool         (*read)(process *p, NPVariant *result);
   bool         (*stop)(process *p);
   void         (*destroy)();
} cmd_shell;

bool init_shell();

extern cmd_shell *shell;

#endif /*_SHELL_H_*/
