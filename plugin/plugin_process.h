#pragma once
#ifndef _PLUGIN_PROCESS_H_
#define _PLUGIN_PROCESS_H_

/* FIXME: pid_t - see FIXME bellow */
#include <unistd.h>

#include "plugin_npapi.h"
#include "plugin_object.h"

/** Plugin process - struct */
typedef struct _process {
   struct _process *next;

   NPObject        *obj;
   NPP             instance;

/* FIXME: Move pid_t and pipe to typedef under unix/ directory */
   pid_t           pid;
   int             pipe[2];

   bool            running;

   bool (*read)(struct _process *p, NPVariant *result);
   bool (*stop)(struct _process *p);

} process;

/** Plugin processes - list */
typedef struct _process_list {
   struct _process *first;

   NPIdentifier    read;
   NPIdentifier    stop;

   void (*destroy)(struct _process_list *l);

} process_list;

/** Plugin processes */
extern process_list *processes;
extern NPClass      processClass;

#endif /*_PLUGIN_PROCESS_H_*/
