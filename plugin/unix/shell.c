#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "debug.h"
#include "shell.h"
#include "npapi.h"
#include "module.h"

/*
EXECVPE() WORKAROUND

The following should work as of glibc 2.11:
char* args[] = { "traceroute", "example.com", NULL };
char* env[] = { "PATH=$PATH:/usr/sbin:/sbin/", NULL };
execvpe(args[0], args, env);

But because of backward compatibility (as for Feb 2010),
we must run `which traceroute' first (as a work-around
to get absolute path of command's executable file):
$ PATH="$PATH:/usr/sbin/:/sbin/" which traceroute
/usr/sbin/traceroute

And then we can finally run:
execv("/usr/sbin/traceroute", args);
*/

cmd_shell  *shell = NULL;                                            /**< Shell abstraction */
char       *buffer = NULL;                                           /**< Buffer for pipes I/O */
char       *which_argv[] = { "/usr/bin/which", NULL/*path*/, NULL }; /**< Arguments for which command */
char       *which_env[] = { NULL/*variables*/, NULL };               /**< Environment for which command */
char       *user_paths = NULL;                                       /**< User paths */
const char *root_paths = ":/usr/sbin:/sbin/";                        /**< Super-user binary paths */

/**
 * Find Unix program path
 *
 * Finds program in user's environment paths, /usr/sbin and /sbin
 *
 * @param program Program name
 * @return String path (dynamically allocated, needs to be freed) on success
 *         NULL on error
 */
static char *
find_program_path(const char *program)
{
   int pipes[2];
   int pid;
   int i;
   int len;
   char *path = NULL;

   /* create pipe for communication */
   if (pipe(pipes) == -1)
      return NULL;

   /* fork the process */
   if ((pid = vfork()) == 0) {
      /* child */

      /* close unused unused read end */
      close(pipes[0]);

      /* redirect stdout to write end */
      if (dup2(pipes[1], 1) == -1)
         _exit(1);

      /* fill which argument by program name */
      which_argv[1] = (char *)program;

      /* execute command
       * which <cmd>
       */
      if (execve(which_argv[0], which_argv, which_env) == -1)
         _exit(1);

      /* won't get here - executed program exits */

   } else if (pid == -1) {
      /* error - can't fork the parent process */

      DEBUG_STR("shell->find(%s): fork() error", program);

      return NULL;

   } else {
      /* parent */

      /* close unused write end */
      close(pipes[1]);

      /* read child data from the pipe */
      if ((len = read(pipes[0], buffer, BUFLEN - 1)) > 0) {
         /* be sure to end string by '\0' character */
         /* we need only first line (end with new line) */
         for (i = 0; i < len && buffer[i] != '\n'; ++i);
         buffer[i] = '\0';
         len = i;

         path = browser->memalloc((len + 1) * sizeof(char));
         memcpy(path, buffer, len + 1);
      } else {
         /* error or zero length */

         DEBUG_STR("shell->find(%s): NULL", program);
         return NULL;
      }

      /* clean child's status from process table */
      waitpid(pid, NULL, 0);

      DEBUG_STR("shell->find(%s): \"%s\"", program, path);

      return path;
   }
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
   kill(p->pid, 9);
   waitpid(p->pid, NULL, 0);

   close(p->pipe[0]);
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
   int len;
   int status;
   NPString str;
   NPUTF8 *chars;

   /* return if the process is not running */
   if (!p->running) {
      DEBUG_STR("shell->read(): false (not running)");
      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }

   /* read from pipe */
   if ((len = read(p->pipe[0], buffer, BUFLEN - 1)) > 0) {
      /* data read successfully */

      /* be sure to terminate string by null-character */
      buffer[len] = '\0';

      /* fill the result string */
      chars = browser->memalloc((len + 1) * sizeof(*chars));
      memcpy(chars, buffer, len);
      STRING_UTF8CHARACTERS(str) = chars;
      STRING_UTF8LENGTH(str) = len;

      result->type = NPVariantType_String;
      result->value.stringValue = str;

      DEBUG_STR("shell->read(): string(len=%d)", len);

      return true;

   } else if (len == -1) {
      /* error */

      if (errno == EAGAIN || errno == EWOULDBLOCK) {
         /* non-blocking reading, continue */

         DEBUG_STR("shell->read(): false (no data, continue)");

         BOOLEAN_TO_NPVARIANT(false, *result);
         return true;

      } else {
         /* error */

         p->running = false;

         DEBUG_STR("shell->read(): false (read error %d)", errno);

         BOOLEAN_TO_NPVARIANT(false, *result);
         return false;
      }

   } else if (len == 0) {
      /* no data available */

      DEBUG_STR("shell->read(): false (end of file) ");

      /* test if child became a zombie */
      waitpid(p->pid, &status, WNOHANG);

      if (WIFEXITED(status)) {
         /* child has finished */

         p->running = false;

         DEBUG_STR("shell->read(): false (process has finished) ");

         /* clean it's status from process table */
         waitpid(p->pid, NULL, 0);

      }

      BOOLEAN_TO_NPVARIANT(false, *result);
      return true;
   }
}

/**
 * Run system command
 *
 * @param p Variable to store process data to
 * @param path Absolute path to the command
 * @param argv Arguments for running process, execv()-style
 * @return True on success, false otherwise
 */
static bool
run_command(process *p, const char *path, char *const argv[])
{
   int flags;

   /* return if the process is already running */
   if (p->running) {
      DEBUG_STR("shell->run(): false (process already running)");
      return false;
   }

   /* create pipe for communication */
   if (pipe(p->pipe) == -1) {
      DEBUG_STR("shell->run(): pipe() error");
      browser->setexception(NULL, "shell->run(): pipe() error");
      goto err_pipe;
   }

   /* fork the process */
   if ((p->pid = vfork()) == 0) {
      /* child */

      /* close read end of pipe */
      close(p->pipe[0]);

      /* stdout and stderr to write end of the pipe */
      if (dup2(p->pipe[1], 1) == -1 || dup2(p->pipe[1], 2) == -1) {
         browser->setexception(NULL, "startCommand(): dup2() error");
         _exit(1);
      }

      /* execute command */
      if (execv(path, argv) == -1) {
         browser->setexception(NULL, "shell->run(): execv() error");
         _exit(1);
      }

      /* won't get here - executed program exits */

   } else if (p->pid == -1) {
      /* error - can't fork the parent process */

      DEBUG_STR("shell->run(): vfork() error");
      browser->setexception(NULL, "shell->run(): vfork() error");
      goto err_fork;

   } else {
      /* parent */
      DEBUG_STR("shell->run(\"%s, ...\"): child pid %d", path, p->pid);

      /* close write end of pipe */
      close(p->pipe[1]);

      /* make read end of pipe non-blocking */
      if ((flags = fcntl(p->pipe[0], F_GETFL)) == -1) {
         DEBUG_STR("shell->run(): fcntl(F_GETFL) error");
         browser->setexception(NULL, "shell->run(): fcntl(F_GETFL) error");
         goto err_fcntl;
      }
      if (fcntl(p->pipe[0], F_SETFL, flags | O_NONBLOCK) == -1) {
         DEBUG_STR("shell->run(): fcntl(F_SETFL) error");
         browser->setexception(NULL, "shell->run(): fcntl(F_SETFL) error");
         goto err_fcntl;
      }

      p->running = true;
   }

   return true;

err_fork:
   close(p->pipe[0]);

err_fcntl:
   close(p->pipe[1]);

err_pipe:
   browser->memfree(p);
   return false;
}

/**
 * Shell_module destructor
 *
 * @param m Shell module to be destroyed
 */
static void
destroy_shell_module(shell_module *m)
{
   if (m) {
      if (m->path)
         browser->memfree(m->path);
      browser->memfree(m);
   }
}

/**
 * Shell_module constructor
 *
 * @param program Command to be run
 * @return Initialized shell_module on success, NULL otherwise
 */
shell_module *
init_shell_module(const char *program)
{
   shell_module *m;

   /* allocate shell module */
   m = browser->memalloc(sizeof(*m));
   if (!m)
      return NULL;

   /* find program Unix path in system */
   m->path = shell->find(program);
   if (m->path)
      m->found = true;
   else
      m->found = false;

   return m;
}

/**
 * Shell destructor
 *
 */
static void
destroy_shell()
{
   DEBUG_STR("shell->destroy()");

   if (buffer != NULL)
      browser->memfree(buffer);
   if (which_env[0] != NULL)
      browser->memfree(which_env[0]);
   if (shell != NULL)
      browser->memfree(shell);
}

/**
 * Shell constructor
 *
 * @return True on success, false otherwise
 */
bool
init_shell()
{
   DEBUG_STR("shell->init()");

   /* allocate shell object */
   if ((shell = browser->memalloc(sizeof(cmd_shell))) == NULL)
      goto err_sh_alloc;

   shell->destroy = destroy_shell;
   shell->find = find_program_path;
   shell->run = run_command;
   shell->init_module = init_shell_module;
   shell->destroy_module = destroy_shell_module;
   shell->stop = process_stop;
   shell->read = process_read;

   /* allocate buffer */
   if ((buffer = browser->memalloc(BUFLEN * sizeof(char))) == NULL)
      goto err_buf_alloc;

   /* get user paths from variable PATH */
   if ((user_paths = getenv("PATH")) == NULL)
      goto err_getenv;

   /* allocate modified PATH variable */
   if ((which_env[0] = browser->memalloc((strlen(user_paths) + strlen(root_paths) + 1) * sizeof(char))) == NULL)
      goto err_env_alloc;

   /* add superuser paths into PATH variable:
      PATH="$PATH:/usr/sbin/:/sbin/"
   */
   memcpy(which_env[0], "PATH=", strlen("PATH="));
   memcpy(which_env[0] + strlen("PATH="), user_paths, strlen(user_paths));
   strncpy(which_env[0] + strlen("PATH=") + strlen(user_paths), root_paths, strlen(root_paths));

   return true;

err_env_alloc:
err_getenv:
   free(buffer);
   buffer = NULL;

err_buf_alloc:
   free(shell);
   shell = NULL;

err_sh_alloc:
   return false;
}
