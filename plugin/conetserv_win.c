#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ctype.h>

#include "conetserv.h"

#define DEBUGCON 0

/* Pipe recources 0 for read 1 for write*/
HANDLE pipes[command_t_count][2];
/* process handles */
HANDLE pids[command_t_count];
/* defines if process is running */
bool isRunning[command_t_count] = {0};

char* cmd_args[command_t_count] = {
   "ping -t",
   "ping -6 -t",
   "tracert",
   "tracert -6",
   "nslookup",
	"nslookup",
};

#define errorExitFunc(msg) {isRunning[cmd]=0; logmsg(msg); npnfuncs->setexception(NULL, msg); return 0;}
#define errorExitChild(msg) {logmsg(msg); npnfuncs->setexception(NULL, msg); ExitProcess(1);}

bool startCommand(command_t cmd, NPUTF8* arg_host)
{
	char cmdchar[100];
	SECURITY_ATTRIBUTES saAttr; 
	PROCESS_INFORMATION procInfo; 
   STARTUPINFOA startInfo;
   BOOL success = FALSE; 
	DWORD status;

	/* check for running state */
   if (isRunning[cmd])
	{
      GetExitCodeProcess( pids[cmd], &status );
		if( status == STILL_ACTIVE )
			return false;
		else
		{
			//Close handles
         CloseHandle(pipes[cmd][0]);
         CloseHandle(pipes[cmd][1]);
		}
	}
	
	isRunning[cmd]=1;

   /*creating command for execution*/
   sprintf(cmdchar, "%s %s", cmd_args[cmd], (char *)arg_host);
	
	/* Set the bInheritHandle flag so pipe handles are inherited. */
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
   saAttr.bInheritHandle = TRUE; 
   saAttr.lpSecurityDescriptor = NULL; 

	/* Create a pipe for the child process's STDOUT. */
   if ( ! CreatePipe(&pipes[cmd][0], &pipes[cmd][1], &saAttr, 0) )
      errorExitFunc("CoNetServ: startCommand(): CreatePipe() - error\n")

	/* Ensure the read handle to the pipe for STDOUT is not inherited. */
   if ( ! SetHandleInformation(pipes[cmd][0], HANDLE_FLAG_INHERIT, 0) )
      errorExitFunc("CoNetServ: startCommand(): SetHandleInformation() - error\n")
	
	/* Create a child process which uses stdout pipe */
	
	/* Prepare structures and set stdout handles */
	ZeroMemory( &procInfo, sizeof(PROCESS_INFORMATION) );
	ZeroMemory( &startInfo, sizeof(STARTUPINFO) );
   startInfo.cb = sizeof(STARTUPINFO); 

	if(!DEBUGCON){
		startInfo.wShowWindow = SW_HIDE;
		startInfo.dwFlags |= STARTF_USESHOWWINDOW;
      startInfo.hStdError = pipes[cmd][1];
      startInfo.hStdOutput = pipes[cmd][1];
		startInfo.dwFlags |= STARTF_USESTDHANDLES;
	}

	success = CreateProcessA(NULL, 
		cmdchar,			// command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags
      NULL,          // use parent's environment 
		NULL,          // current directory 
      &startInfo,		// STARTUPINFO pointer 
      &procInfo);		// receives PROCESS_INFORMATION 
	
	if(!success)
	{
		isRunning[cmd] = 0;
		errorExitFunc("CoNetServ: startCommand(): CreateProcess() - error\n")
	}
	else
	{
		logmsg("CoNetServ: startCommand(): CreateProcess() - success\n");
		/* store process handle and close process thread handle */
      pids[cmd] = procInfo.hProcess;
		CloseHandle(procInfo.hThread);
	}
   return true;
}

bool stopCommand(command_t cmd)
{
   logmsg("CoNetServ: stopCommand()\n");

	/* kill the command, if running */
	if(isRunning[cmd])
	{
      TerminateProcess(pids[cmd], 0);
      CloseHandle(pipes[cmd][0]);
      CloseHandle(pipes[cmd][1]);
		isRunning[cmd] = 0;
		return true;
	}
	else
	return false;
}

/* FIXME Supports only ASCII and CP1250 command lines. */
int readCommand(command_t cmd, NPUTF8 *_buf)
{
   char* buf = (char *)_buf;
	unsigned i;
	DWORD read = 0;
   BOOL success = FALSE;
	DWORD status;
	/* check for running state */
   if (isRunning[cmd])
	{
		/* check for data on pipes */
      PeekNamedPipe(pipes[cmd][0], NULL, 0, NULL, &status, NULL);
		if(status)
		{
         success = ReadFile( pipes[cmd][0], buf, BUFFER_LENGTH - 1, &read, NULL);
			if( ! success || read == 0 ) 
				read = 0;
			else	//on success
			{
				
				for(i=0; i < read; i++)
				{
					switch((unsigned char)buf[i])
					{
					case 0xFD:		//ř
						buf[i]='r';
						break;
					case 0xA1:		//í
						buf[i]='i';
						break;
					case 0x85:		//ů
						buf[i]='u';
						break;
					case 0xD8:		//ě
						buf[i]='e';
						break;
					case 0xD4:		//ď
						buf[i]='d';
						break;
					case 159:		//č
						buf[i]='c';
						break;
					case 0xE7:		//š
						buf[i]='s';
						break;
					case 0xEC:		//ý
						buf[i]='y';
						break;
					case 0xA7:		//ž
						buf[i]='z';
						break;
					case 0xA0:		//á
						buf[i]='a';
						break;
					case 0x82:		//é
						buf[i]='e';
						break;
					default:
						break;
					}
				}
				buf[read] = '\0';
				logmsg(buf);
			}
		}

      GetExitCodeProcess( pids[cmd], &status );
		if(status != STILL_ACTIVE )
		{
			/*check for any extra data*/
         PeekNamedPipe(pipes[cmd][0], NULL, 0, NULL, &status, NULL);
			if(!status)
			{
				/* Close handles */
            CloseHandle(pipes[cmd][0]);
            CloseHandle(pipes[cmd][1]);
				isRunning[cmd] = 0;
			}
		}
		return read;
	}
	else return -1;

	
}
