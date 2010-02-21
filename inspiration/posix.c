#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

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

//SETTINGS
//FILES
char *fileNames[PROGCOUNT] = { "ping.tmp", "trace.tmp", "whois.tmp" };
char *commandNames[PROGCOUNT] = { "ping -c 10 ", "traceroute ", "whois " };
//PING
unsigned repeats = 10;

//position in files
unsigned position[PROGCOUNT] = {0};

//Global variables for threads
pthread_t threads[PROGCOUNT];		//field for saving thread
threadData_t threadData[PROGCOUNT];	//field for thread data
int running[PROGCOUNT] = {0};			//defines, if thread is running

//function for running new thread
void *runExternalProg(void* args);

int startPing(char* address);
int startTrace(char* address);
int startWhois(char* address);
int startPlugin(unsigned ident, char* address);

char* getDataPing();
char* getDataTrace();
char* getDataWhois();
char* getData(unsigned ident);

int stopPing(void);
int stopTrace(void);
int stopWhois(void);
int stopPlugin(unsigned ident);

int main()
{
  int i;
  printf("Main thread pid: %d\n", getpid());
  startPing("www.seznam.cz");
  
  usleep(100000);
  
  while(1)
  {
    char* tmp;
    usleep(500000);
    
    for(i = 0; i < PROGCOUNT; i++)
    {
      if(running[i]&&(tmp = getData(i)))
      {
	printf("%s",tmp);
      }
    }
  }
  
  pthread_join(threads[PING], NULL);
  
  pthread_exit(NULL);
}

void *runExternalProg(void* args)
{
  
  threadData_t* data= (threadData_t*) args;
  
  if(DEBUG)
    fprintf(stderr,"Created thread id: %d pid: %d ppid: %d\n",data->id, getpid(), getppid());
  
  //freopen( data->fName, "w", stdout );	//redirect stdout to a file
  
  system(data->command);
  
  //free allocated data
  free(data->command);

  //erase sign of running
  running[data->id] = 0;
  
  //reset position in file
  position[data->id] = 0;
  //exit thread
  pthread_exit(NULL);
}

int startPing(char* address){
  return startPlugin(PING, address);
}

int startTrace(char* address){
  return startPlugin(TRACE, address);
}

int startWhois(char* address){
  return startPlugin(WHOIS, address);
}

int startPlugin(unsigned ident, char* address)
{
  if(running[ident])	//check if operation is alerady running
    return 0;
  running[ident] = 1;
  int i;
  int rc;
  void* status;
  char* tmp;
  
  pthread_attr_t attr;
  tmp = malloc(strlen(address) + 30);
  
  //build command
  strcpy(tmp, commandNames[ident]);
  strcat(tmp, address);
  strcat(tmp, " > ");
  strcat(tmp, fileNames[ident]);
  threadData[ident].command = tmp;
  
  /* Initialize and set thread detached attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  //create new thread and send him data 
  threadData[ident].id = ident;
  rc = pthread_create(&threads[ident], &attr, runExternalProg, (void*) &threadData[ident]);
  if(rc)
  {
    perror("Failed to create thread");
    return 0;
  }
  
  pthread_attr_destroy(&attr);

  return 1;
}  

char* getDataPing(){
  return getData(PING);
}

char* getDataTrace(){
  return getData(TRACE);
}

char* getDataWhois(){
  return getData(WHOIS);
}

char* getData(unsigned ident)
{ 
  unsigned blockCount = 0;		//number of allocated blocks
  unsigned length = 0;			//length of string
  char* buffer = NULL;			//string for sending
  FILE* input;
  input = fopen(fileNames[ident], "r");
  
  char* ok;
  
  fseek(input, 0, SEEK_END);
  //check for any new data
  if(ftell(input) < position[ident])
  {
    fclose(input);
    return NULL;
  }
  else
  {	//if there is something new, move to it, read it and send it
    fseek(input, position[ident], SEEK_SET);
    
    do{
      if(length+BLOCKSIZE>=blockCount*BLOCKSIZE)
	buffer = (char*) realloc(buffer, ++blockCount*BLOCKSIZE);
      ok = fgets(buffer+length, BLOCKSIZE, input);
      length += (ok? strlen(ok) : 0);			
      //if(DEBUG)
//	fprintf(stderr, "Read: %d bytes\n", (ok? strlen(ok) : 0));
      
    }while(ok!=NULL);
    //move position to current location
    position[ident] = ftell(input);
    fclose(input);
    return buffer;
  }
  
}

int stopPing(void){
  stopPlugin(PING);
}

int stopTrace(void){
  stopPlugin(TRACE);
}

int stopWhois(void){
  stopPlugin(WHOIS); 
}

int stopPlugin(unsigned ident)
{
  FILE* shpid;
  char com[200];
  int tokill;
  pid_t pid;
 
  pid = getpid();
  //use ps, grep and awk for getting to know, which process has this program as a parent
  //first get id of sh
  sprintf(com, "ps -ef | grep -e '%s' | grep -v -e grep | awk '{if ( $3 == %d ) print $2 }' > shpid",commandNames[ident], pid);
  system(com);
  
  if(!(shpid = fopen("shpid", "r")))
    return 0;
  
  fscanf(shpid, "%d\n", &tokill);
  //then of program and kill him
  sprintf(com, "ps -ef | grep -e '%s' | grep -v -e grep | awk '{if ( $3 == %d ) print $2 }' | xargs kill -9 ", commandNames[ident], tokill); 
  system(com);
  
  fclose(shpid);  
  
  running[ident] = 0;
  //stop the thread  
  pthread_kill(threads[ident], SIGKILL);
  return 1;
}

