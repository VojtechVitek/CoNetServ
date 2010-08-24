#pragma once
#ifndef _PLUGIN_PROCESS_H_
#define _PLUGIN_PROCESS_H_

#include "npapi.h"
#include "plugin.h"

/** Windows: Plugin process - struct */
typedef struct _process {
   NPObject        obj;
   bool            running;

   /* Platform-dependent data: */
   HANDLE          pid;
   HANDLE          pipe[2];
} process;

extern NPClass      processClass;

#endif /*_PLUGIN_PROCESS_H_*/
