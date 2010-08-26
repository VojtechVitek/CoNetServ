#pragma once
#ifndef _CMD_EXE_H_
#define _CMD_EXE_H_

#include "npapi.h"
#include "module.h"
#include "process.h"

#define BUFLEN       1024
#define BUFLEN_WCHAR (4 * BUFLEN)
#define BUFLEN_UTF8  (4 * BUFLEN)

typedef struct _cmd_exe_module {
   module       m;
   bool         found;
} cmd_exe_module;

typedef struct _cmd_exe {
   cmd_exe_module *(*init_module)(const char* name);
   void           (*destroy_module)(cmd_exe_module *m);
   bool           (*run)(process *p, const char *cmd);
   bool           (*read)(process *p, NPVariant *result);
   bool           (*stop)(process *p);
   void           (*destroy)();
} cmd_exe;

bool init_cmd_line();

extern cmd_exe *cmd_line;

#endif /*_CMD_EXE_H_*/
