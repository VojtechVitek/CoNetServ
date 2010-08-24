#pragma once
#ifndef _CMD_EXE_H_
#define _CMD_EXE_H_

typedef struct _cmd_exe_module {
   module       m;
   bool         found;
   //char         *path;
} cmd_exe_module;

typedef struct _cmd_exe {
   cmd_exe_module *(*init_module)(const char* name);
   void           (*destroy_module)(cmd_exe_module *m);
   bool           (*run)(process *p, const char *path, char *const argv[]);
   bool           (*read)(process *p, NPVariant *result);
   bool           (*stop)(process *p);
   void           (*destroy)();
} cmd_exe;

bool init_cmd_exe();

extern cmd_shell *shell;

#endif /*_CMD_EXE_H_*/
