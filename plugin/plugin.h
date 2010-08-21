#pragma once
#ifndef _PLUGIN_OBJECT_H_
#define _PLUGIN_OBJECT_H_

#include "npapi.h"

typedef struct _object {
   NPObject obj;
   NPP      instance;
} object;

extern NPClass          pluginClass;

#endif /*_PLUGIN_OBJECT_H_*/