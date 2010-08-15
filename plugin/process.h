#pragma once
#ifndef _PLUGIN_PROCESS_H_
#define _PLUGIN_PROCESS_H_

/* FIXME: pid_t - see FIXME bellow */
#include <unistd.h>

#include "npapi.h"
#include "plugin.h"

/** Plugin process - struct */
typedef struct _process {
   NPObject        obj;
//   NPP             instance;

/* FIXME: Move pid_t and pipe to typedef under unix/ directory */
   bool            running;
   pid_t           pid;
   int             pipe[2];
} process;

extern NPClass      processClass;

#endif /*_PLUGIN_PROCESS_H_*/
