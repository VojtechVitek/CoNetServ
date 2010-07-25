#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <windows.h>

#include "plugin.h"

#define DEBUGCON 0
#define strcasecmp stricmp
#define strncasecmp strnicmp

#define LOCALEDIR "."

#if _MSC_VER && !__cplusplus
# define inline __inline
#endif

/* Pipe recources 0 for read 1 for write*/
HANDLE pipes[command_t_count][2];
/* process handles */
HANDLE pids[command_t_count];
/* defines if process is running */
bool isRunning[command_t_count] = {0};

char* cmd_args[command_t_count] = {
   "ping",
   "ping -6 -t",
   "tracert",
   "tracert -6",
   "nslookup",
   "nslookup",
   "chcp 65001 & ping www.seznam.cz",
};

#define errorExitFunc(msg) {isRunning[cmd]=0; logmsg(msg); npnfuncs->setexception(NULL, msg); return 0;}
#define errorExitChild(msg) {logmsg(msg); npnfuncs->setexception(NULL, msg); ExitProcess(1);}

bool startCommand(command_t cmd, NPUTF8* arg_host)
{
	char cmdchar[100];
	SECURITY_ATTRIBUTES saAttr;
	PROCESS_INFORMATION procInfo;
	STARTUPINFO startInfo;
	BOOL success = FALSE;
	DWORD status;

	LPWSTR utfstring;
	int length;

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
   if(cmd == LOCALE) {
      sprintf(cmdchar, "cmd.exe /U /C \"%s\"", cmd_args[cmd]);
   }
   else
      sprintf(cmdchar, "cmd.exe /U /C \"%s %s\"", cmd_args[cmd], (char *)arg_host);

   /* Prepare data to unicode string for passing to createProccessW function */
   length = MultiByteToWideChar(CP_UTF8, 0, cmdchar, -1, NULL, 0);
   utfstring = (LPWSTR) malloc(length * sizeof(WCHAR) + 1);

   MultiByteToWideChar(CP_UTF8, 0, cmdchar, -1, utfstring, length);

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

	success = CreateProcess(NULL,
		cmdchar,			// command line
		NULL,          // process security attributes
		NULL,          // primary thread security attributes
		TRUE,          // handles are inherited
		0,             // creation flags
		NULL,          // use parent's environment
		NULL,          // current directory
		&startInfo,		// STARTUPINFO pointer
		&procInfo		// receives PROCESS_INFORMATION
   );

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
   {
      return false;
   }
}

int readCommand(command_t cmd, NPUTF8 *_buf)
{
	char buf[BUFFER_LENGTH];
	DWORD len = 0;
	DWORD status;

	LPWSTR utfstring;
	int length;

	UINT codepage;


	/* check if running */
	if (isRunning[cmd])
	{
      /* check for data on pipes */
      PeekNamedPipe(pipes[cmd][0], NULL, 0, NULL, &status, NULL);
		if(status)
		{
			if (!ReadFile( pipes[cmd][0], buf, BUFFER_LENGTH - 1, &len, NULL))
			{
				len = 0;
			}
		}

		buf[len] = '\0';

		if(len) {
			/* Get active codepage id */
			codepage = GetOEMCP();

			/* Convert data first to multibyte and then to utf-8 */
			len = MultiByteToWideChar(codepage, 0, buf, -1, NULL, 0);
			utfstring = (LPWSTR) malloc(len * sizeof(WCHAR) + 2);
			utfstring[len] = 0;

			MultiByteToWideChar(codepage, 0, buf, -1, utfstring, len);	
			//!!TODO needs more atention - not sure if we are not losing data here, needs checking
			len = WideCharToMultiByte (CP_UTF8, 0, utfstring, len, 0, 0, 0, 0);
      
			len = len > BUFFER_LENGTH - 1 ? BUFFER_LENGTH - 1 : len;
			_buf[len] = 0;

			WideCharToMultiByte (CP_UTF8, 0, utfstring, -1, _buf, len - 1, 0, 0);
			//sprintf(_buf, "%d", codepage, utfstring[0]);
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

		return len;
	}
	else
   {
      return -1;
   }
}
