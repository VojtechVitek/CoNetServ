#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROGCOUNT 3
#define BUFLENGHT 80
#define WAITTIME 1

enum
{
  PING 		= 0,
  TRACE 	= 1,
  WHOIS 	= 2
};

typedef int pipe_t[2];

int child(pipe_t pipes, unsigned char type);
int parent(pid_t child[PROGCOUNT], pipe_t pipes[PROGCOUNT]);

int main()
{
  int i;
  pipe_t pipes[PROGCOUNT];	//declaration of pipes
  
  pipe(pipes[PING]);
  pipe(pipes[TRACE]);
  pipe(pipes[WHOIS]);
  
  //printf("pipe1: %d %d ; pipe2: %d %d\n",pipes[PING][0], pipes[PING][1], pipes[TRACE][0], pipes[TRACE][1]);
  //return;
  //for(i=0; i<PROGCOUNT; i++)
  //  pipe(pipes[i]);
  
  pid_t pids[PROGCOUNT];	//structure for saving child pids
  pid_t pID;
  
  for(i = 0; i< PROGCOUNT; i++)
  {
    pID = fork();		//forking program
    if(pID == 0)
    {
      child(pipes[i], i);	//run child function with according type of program
      return 0;
    }
    else if(pID < 0)
      return 1;
    else
    {			
      pids[i] = pID;		//add pid to structure for parent
      //usleep(1000000); 
      if(i==PROGCOUNT-1)	//check for last program
      	parent(pids,pipes);	//call parent function
    }
  }
  return 0;
}

int child(pipe_t pipes, unsigned char type)
{
  fprintf(stderr, "%d", type);
  close(pipes[0]);
  dup2(pipes[1],1);	//assign stdout to a pipe  
  
  switch(type)
  {
    case PING:
      fprintf(stderr,"ping: %d %d\n",pipes[0],pipes[1]);
      execlp("ping", "ping", "www.seznam.cz", "-c", "10", NULL);	//replacing program with ping
      break;
    case TRACE:
      fprintf(stderr,"trace: %d %d\n",pipes[0],pipes[1]);
      exit(0);
      break;
    case WHOIS:
      fprintf(stderr,"whois: %d %d\n",pipes[0],pipes[1]);
      execlp("whois", "whois", "www.seznam.cz", NULL);			//replacing program with whois
      exit(0);
      break;
    default:
      break;
  }
  return 1;
}

int parent(pid_t child[PROGCOUNT], pipe_t pipes[PROGCOUNT])
{
  int i,running;
  int status;			//data for pipes
  ssize_t nbytes;
  char buffer[BUFLENGHT];
  
  fd_set rfds;			//data for select()
  struct timeval tv;
  int retval;
  
  //close all output-pipes
  for(i = 0; i<PROGCOUNT; i++)
    close(pipes[i][1]);
  
  //inicialize structures for select
  FD_ZERO(&rfds);
  for(i=0; i<PROGCOUNT; i++)
    FD_SET(pipes[i][0], &rfds);
  
  //inicialize timer
  tv.tv_sec = WAITTIME;
  tv.tv_usec = 0;
    
  
  while(1)
  {  
    retval = select(pipes[WHOIS][0]+1, &rfds, NULL, NULL, &tv);
    if(retval == -1)
      perror("Error in select()");
    else if(retval)
    {
      for(i = 0; i<PROGCOUNT; i++)	//check, which pipe is active
      {
	if(FD_ISSET(pipes[i][0], &rfds))
	{
	  nbytes = read(pipes[i][0], buffer, BUFLENGHT);		//read bytes from input pipe
	  if(nbytes>0)
	  {
	    buffer[nbytes]='\0';
	    printf("%s",buffer);
	  }
	  for(i=0; i<PROGCOUNT; i++)
	    FD_SET(pipes[i][0], &rfds);
	}
      }
      tv.tv_sec=WAITTIME;			//reset timer
      tv.tv_usec=0;
      
    }
    else		//check if all of children are still alive and then exit
    {   
      running=0;
      for(i=0; i<PROGCOUNT; i++)
      {
	if(!waitpid(child[i], &status, WNOHANG))
	{
	  tv.tv_sec = WAITTIME;    
	  tv.tv_usec = 0;
	  for(i=0; i<PROGCOUNT; i++)
	    FD_SET(pipes[i][0], &rfds);
	  
	  running=1;
	}
      }
      if(!running)
	break;
    }
  }
  
  for(i = 0; i<PROGCOUNT; i++)
    close(pipes[i][0]);
  
  return 1;
}