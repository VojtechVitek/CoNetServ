#ifndef _CONETSERV_POSIX_H_
#define _CONETSERV_POSIX_H_

#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

typedef enum {
   /* system commands: */
   PING = 0,
   TRACE,
   WHOIS,

   command_t_count

   /* implemented commands: */
} command_t;

bool startCommand(command_t cmd, char* addr);
int readCommand(command_t cmd, char* buf);
bool stopCommand(command_t cmd);

#endif /*_CONETSERV_POSIX_H_*/
