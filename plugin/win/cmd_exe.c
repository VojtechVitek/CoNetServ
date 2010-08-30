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

/* Disable deprecation messages */
#define _CRT_SECURE_NO_WARNINGS
#define LOCALEDIR "."

#if _MSC_VER && !__cplusplus
#define inline __inline
#endif

cmd_exe *cmd_line = NULL;     /**< Command line abstraction */
char    *buffer = NULL;       /**< Buffer for pipe I/O */
char    *buffer_wchar = NULL; /**< Buffer for wide chars */
char    *buffer_utf8 = NULL;  /**< Buffer for utf8 chars */

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

   /* close read pipe */
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
static bool
process_read(process *p, NPVariant *result)
{
   DWORD len = 0, status;
   UINT codepage;
   NPString str;
   NPUTF8 *chars;

   /* return if the process is not running */
   if (!p->running) {
      DEBUG_STR("cmd_line->read(): false (not running)");
      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }

   /* peek - check if data are available */
   if (!PeekNamedPipe(p->pipe[0], NULL, 0, NULL, &status, NULL)) {

      DEBUG_STR("cmd_line->read(): false (peek error %d)", GetLastError());

      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;

   }

   /* can't peek */
   if (!status) {

      GetExitCodeProcess(p->pid, &status);
      if (status != STILL_ACTIVE) {

         DEBUG_STR("cmd_line->read(): false (process has finished) ");

         /* close read pipe */
         CloseHandle(p->pipe[0]);
         p->running = false;

      } else {

         DEBUG_STR("cmd_line->read(): false (no data, continue)", GetLastError());

      }

      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }

   /* read from pipe */
   if (!ReadFile(p->pipe[0], buffer, BUFLEN - 1, &len, NULL)) {

      DEBUG_STR("cmd_line->read(): false (read error %d)", GetLastError());

      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }

   if (len <= 0) {
      DEBUG_STR("cmd_line->read(): false (string len=0)", GetLastError());

      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }

   /* get active codepage id */
   codepage = GetOEMCP();

   /* convert multibyte characters (of current codepage) to widechar characters */
   len = MultiByteToWideChar(codepage, 0, buffer, len, buffer_wchar, BUFLEN_WCHAR);

   /* convert widechar characters to utf-8 */
   len = WideCharToMultiByte(CP_UTF8, 0, buffer_wchar, len, buffer_utf8, BUFLEN_UTF8, 0, 0);

   /* be sure to terminate string by null-character */
   buffer_utf8[len] = '\0';

   /* allocate and fill the result string */
   chars = browser->memalloc((len + 1) * sizeof(*chars));
   memcpy(chars, buffer_utf8, len + 1);
   STRING_UTF8CHARACTERS(str) = chars;
   STRING_UTF8LENGTH(str) = len;

   result->type = NPVariantType_String;
   result->value.stringValue = str;

   DEBUG_STR("cmd_line->read(): string(len=%d)", len);

   return true;
}

/**
 * Run system command
 *
 * @param p Variable to store process data to
 * @param path Command name
 * @param argv String of arguments
 * @return True on success, false otherwise
 */
static bool
run_command(process *p, const char *cmd)
{
   SECURITY_ATTRIBUTES saAttr;
   PROCESS_INFORMATION procInfo;
   STARTUPINFO startInfo;
   BOOL success = FALSE;
   DWORD status;
   char *command;
   int len;

   /* return if the process is already running */
   if (p->running) {
      DEBUG_STR("cmd_line->run(): false (process already running)");
      return false;
   }

   /* Set the bInheritHandle flag so pipe handles are inherited. */
   saAttr.nLength = sizeof(saAttr);
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;

   /* Create a pipe for the child process's STDOUT. */
   if (!CreatePipe(&p->pipe[0], &p->pipe[1], &saAttr, 0)) {
      DEBUG_STR("cmd_line->run(): CreatePipe() - error");
      browser->setexception(NULL, "cmd_line->run(): CreatePipe() - error");
      return 0;
   }

   /* Ensure the read handle to the pipe for STDOUT is not inherited. */
   if (!SetHandleInformation(p->pipe[0], HANDLE_FLAG_INHERIT, 0)) {
      DEBUG_STR("cmd_line->run(): SetHandleInformation() - error");
      browser->setexception(NULL, "cmd_line->run(): SetHandleInformation() - error");
      return 0;
   }

   /* Create a child process which uses stdout pipe */

   /* Prepare structures and set stdout handles */
   ZeroMemory(&procInfo, sizeof(procInfo));
   ZeroMemory(&startInfo, sizeof(startInfo));
   startInfo.cb = sizeof(startInfo);

   if (!DEBUGCON) {
      startInfo.wShowWindow = SW_HIDE;
      startInfo.dwFlags |= STARTF_USESHOWWINDOW;
      startInfo.hStdError = p->pipe[1];
      startInfo.hStdOutput = p->pipe[1];
      startInfo.dwFlags |= STARTF_USESTDHANDLES;
   }

   /* create command for execution */
   len = strlen(cmd);
   command = browser->memalloc(sizeof(*command) * (len + 16/*len of bellow cmd*/ + 1));
   sprintf(command, "cmd.exe /U /C \"%s\"", cmd);

   success = CreateProcess(NULL,
      command,       // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      0,             // creation flags
      NULL,          // use parent's environment
      NULL,          // current directory
      &startInfo,    // STARTUPINFO pointer
      &procInfo      // receives PROCESS_INFORMATION
   );

   /* free executed command string */
   browser->memfree(command);

   if (!success) {
      DEBUG_STR("cmd_line->run(): CreateProcess() - error");
      browser->setexception(NULL, "cmd_line->run(): CreateProcess() - error");
      return false;
   }

   /* store process handle and close process thread handle */
   p->pid = procInfo.hProcess;
   p->running = true;

   DEBUG_STR("cmd_line->run(\"%s\"): PID %d", cmd, p->pid);

   CloseHandle(procInfo.hThread);
   return true;
}

/**
 * CMD_EXE_module destructor
 *
 * @param m CMD_EXE_module to be destroyed
 */
static void
destroy_cmd_line_module(cmd_exe_module *m)
{
   if (m) {
      browser->memfree(m);
   }
}

/**
 * CMD_EXE_module constructor
 *
 * @param program Command to be run
 * @return Initialized CMD_EXE_module on success, NULL otherwise
 */
cmd_exe_module *
init_cmd_line_module(const char *program)
{
   cmd_exe_module *m;

   /* allocate cmd_exe module */
   m = browser->memalloc(sizeof(*m));
   if (!m)
      return NULL;

   return m;
}

/**
 * cmd_exe destructor
 *
 */
static void
destroy_cmd_line()
{
   DEBUG_STR("cmd_line->destroy()");

   if (buffer != NULL)
      browser->memfree(buffer);

   if (buffer_wchar != NULL)
      browser->memfree(buffer_wchar);

   if (buffer_utf8 != NULL)
      browser->memfree(buffer_utf8);

   if (cmd_line != NULL)
      browser->memfree(cmd_line);
}

/**
 * CMD_EXE constructor
 *
 * @return True on success, false otherwise
 */
bool
init_cmd_line()
{
   DEBUG_STR("cmd_line->init()");

   /* allocate cmd_exe object */
   if ((cmd_line = browser->memalloc(sizeof(*cmd_line))) == NULL)
      goto err_cmd_alloc;

   cmd_line->destroy = destroy_cmd_line;
   cmd_line->run = run_command;
   cmd_line->init_module = init_cmd_line_module;
   cmd_line->destroy_module = destroy_cmd_line_module;
   cmd_line->stop = process_stop;
   cmd_line->read = process_read;

   /* allocate buffer */
   if ((buffer = browser->memalloc(BUFLEN * sizeof(*buffer))) == NULL)
      goto err_buf_alloc;

   /* allocate buffer for wide chars */
   if ((buffer_wchar = browser->memalloc(BUFLEN_WCHAR * sizeof(*buffer_wchar))) == NULL)
      goto err_buf_wchar_alloc;

   /* allocate buffer for utf8 chars */
   if ((buffer_utf8 = browser->memalloc(BUFLEN_UTF8 * sizeof(*buffer_utf8))) == NULL)
      goto err_buf_utf8_alloc;

   return true;

err_buf_utf8_alloc:
   free(buffer_wchar);
   buffer_wchar = NULL;

err_buf_wchar_alloc:
   free(buffer);
   buffer = NULL;

err_buf_alloc:
   free(cmd_line);
   cmd_line = NULL;

err_cmd_alloc:
   return false;
}
