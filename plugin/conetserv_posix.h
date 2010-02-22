#ifndef _CONETSERV_POSIX_H_
#define _CONETSERV_POSIX_H_

#include <stdbool.h>

#define DEBUG 1
#define PROGCOUNT 3
#define BLOCKSIZE 80

typedef struct{		//structure for data passed to a thread
  int id;
  char *command;
}threadData_t;

enum
{
  PING 		= 0,
  TRACE 	= 1,
  WHOIS 	= 2
};


//function for running new thread
void *runExternalProg(void* args);

bool startPing(char* address);
bool startTrace(char* address);
bool startWhois(char* address);
bool startPlugin(unsigned ident, char* address);

char* readPing();
char* readTrace();
char* readWhois();
char* readPlugin(unsigned ident);

bool stopPing(void);
bool stopTrace(void);
bool stopWhois(void);
bool stopPlugin(unsigned ident);

#endif /*_CONETSERV_POSIX_H_*/
