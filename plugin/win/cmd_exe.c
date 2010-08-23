#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <windows.h>

#include "debug.h"
#include "cmd_exe.h"
#include "npapi.h"
#include "module.h"

/* FIXME: Move these magic definitions to special header file: */
#define DEBUGCON 0
#define strcasecmp stricmp
#define strncasecmp strnicmp

#define LOCALEDIR "."

#if _MSC_VER && !__cplusplus
# define inline __inline
#endif

cmd_exe *cmd_line = NULL; /**< Shell abstraction */
char    *buffer = NULL;   /**< Buffer for pipes I/O */

/**
 * Run system command
 *
 * @param p Variable to store process data to
 * @param path Command name
 * @param argv String of arguments
 * @return True on success, false otherwise
 */
static bool
run_command(process *p, const char *cmd, const char argv)
{
   char cmdchar[100];
   SECURITY_ATTRIBUTES saAttr;
   PROCESS_INFORMATION procInfo;
   STARTUPINFO startInfo;
   BOOL success = FALSE;
   DWORD status;

   /* return if the process is already running */
   if (p->running) {
      DEBUG_STR("shell->run(): false (process already running)");
      return false;
   }
   GetExitCodeProcess(p->pid, &status);
   if (status == STILL_ACTIVE) {
      DEBUG_STR("shell->run(): false (process already running/active)");
      return false;
   }

   /*creating command for execution*/
   sprintf(cmdchar, "cmd.exe /U /C \"%s %s\"", cmd_args[cmd], (char *)arg_host);

   /* Set the bInheritHandle flag so pipe handles are inherited. */
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;

   /* Create a pipe for the child process's STDOUT. */
   if (!CreatePipe(&p->pipe[0], &p->pipe[1], &saAttr, 0)) {
      p->running = false;
      logmsg(msg);
      browser->setexception(NULL, "run_command(): CreatePipe() - error");
      return 0;
   (")
   }

      /* Ensure the read handle to the pipe for STDOUT is not inherited. */
      if ( ! SetHandleInformation(p->pipe[0], HANDLE_FLAG_INHERIT, 0) )
         isRunning[cmd]=0; logmsg(msg); browser->setexception(NULL, msg); return 0;("startCommand(): SetHandleInformation() - error\n")

         /* Create a child process which uses stdout pipe */

         /* Prepare structures and set stdout handles */
         ZeroMemory( &procInfo, sizeof(PROCESS_INFORMATION) );
   ZeroMemory( &startInfo, sizeof(STARTUPINFO) );
   startInfo.cb = sizeof(STARTUPINFO);

   if(!DEBUGCON){
      startInfo.wShowWindow = SW_HIDE;
      startInfo.dwFlags |= STARTF_USESHOWWINDOW;
      startInfo.hStdError = p->pipe[1];
      startInfo.hStdOutput = p->pipe[1];
      startInfo.dwFlags |= STARTF_USESTDHANDLES;
   }

   success = CreateProcess(NULL,
      cmdchar,         // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      0,             // creation flags
      NULL,          // use parent's environment
      NULL,          // current directory
      &startInfo,      // STARTUPINFO pointer
      &procInfo      // receives PROCESS_INFORMATION
      );

   if(!success)
   {
      isRunning[cmd] = 0;
      isRunning[cmd]=0; logmsg(msg); browser->setexception(NULL, msg); return 0;("startCommand(): CreateProcess() - error\n")
   }
   else
   {
      DEBUG_STR("startCommand(): CreateProcess() - success");
      /* store process handle and close process thread handle */
      pids[cmd] = procInfo.hProcess;
      CloseHandle(procInfo.hThread);
   }
   return true;
}

/**
 * Stop running process
 *
 * @param p Process to be stopped
 * @return True on success, false otherwise
 */
static bool
process_stop(process *p)
{
   /* return if the process is not running */
   if (!p->running) {
      DEBUG_STR("process->stop(pid %d): false (not running)", p->pid);
      return false;
   }

   /* kill the process */
   TerminateProcess(p->pid, 0);

   CloseHandle(p->pipe[0]);
   p->running = false;

   DEBUG_STR("process->stop(pid %d): true", p->pid);

   return true;
}

/**
 * Read process output data
 *
 * @param p Process to read data from
 * @param result Result string on success, false otherwise
 * @return True on success, false otherwise
 */
int
process_read(process *p, NPVariant *result)
{
   DWORD len = 0;
   DWORD len2 = 0;
   DWORD status;

   LPWSTR utfstring;

   UINT codepage;

   /* return if the process is not running */
   if (!p->running) {
      DEBUG_STR("shell->read(): false (not running)");
      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }

   /* read from pipe */
   PeekNamedPipe(p->pipe[0], NULL, 0, NULL, &status, NULL);
   if(status)
   {
      if (!ReadFile( p->pipe[0], buf, BUFLEN / 2 - 1, &len, NULL))
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
      utfstring = (LPWSTR) browser->memalloc(len * sizeof(WCHAR) + 2);
      utfstring[len] = 0;

      MultiByteToWideChar(codepage, 0, buf, -1, utfstring, len);
      len2 = WideCharToMultiByte (CP_UTF8, 0, utfstring, len, 0, 0, 0, 0);

      _buf[len2-1] = 0;

      WideCharToMultiByte (CP_UTF8, 0, utfstring, len-1, _buf, len2 - 1, 0, 0);

      browser->memfree(utfstring);
   }

   GetExitCodeProcess( pids[cmd], &status );
   if(status != STILL_ACTIVE )
   {
      /*check for any extra data*/
      PeekNamedPipe(p->pipe[0], NULL, 0, NULL, &status, NULL);
      if(!status)
      {
         /* Close handles */
         CloseHandle(p->pipe[0]);
         CloseHandle(p->pipe[1]);
         isRunning[cmd] = 0;
      }
   }

   return len2;
}
