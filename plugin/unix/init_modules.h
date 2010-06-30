#pragma once
#ifndef _INIT_MODULES_H_
#define _INIT_MODULES_H_

void init_module_ping();

#define INIT_MODULES()      \
   do {                     \
      init_module_ping();   \
   } while (0);

#endif /*_INIT_MODULES_H_*/