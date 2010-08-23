#pragma once
#ifndef _PLUGIN_PROCESS_H_
#define _PLUGIN_PROCESS_H_

#include <unistd.h> /*pid_t*/

#include "npapi.h"
#include "plugin.h"

/** UNIX: Plugin process - struct */
typedef struct _process {
   NPObject        obj;
   bool            running;

   /* Platform-dependent data: */
   pid_t           pid;
   int             pipe[2];
} process;

extern NPClass      processClass;

#endif /*_PLUGIN_PROCESS_H_*/
