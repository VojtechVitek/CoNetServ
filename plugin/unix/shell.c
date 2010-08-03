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
#include "plugin_npapi.h"
#include "plugin_module.h"

/*
EXECVPE() WORKAROUND

This should work from glibc 2.11, where execvpe() is implemented:
char* env[] = { "PATH=$PATH:/usr/sbin:/sbin/", NULL };
args[cmd][1] = addr;
execvpe(args[cmd][0], args[cmd], env);

But as for now (Feb 2010), we must run `which <cmd>` as work-around:
$ PATH="$PATH:/usr/sbin/:/sbin/" which <cmd>
/usr/sbin/<cmd>

-- V-Teq
*/

cmd_shell *shell = NULL;

char       *buffer = NULL;
char       *which_argv[] = { "/usr/bin/which", NULL, NULL};
char       *which_env[] = { NULL, NULL };
char       *user_paths = NULL;
const char *root_paths = ":/usr/sbin:/sbin/";

static char* find_program_path(char *program)
{
   int pipes[2];
   int pid;
   int len;

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
      if ((len = read(pipes[0], buffer, BUFLEN - 1)) == -1 || len == 0) {
         /* Error or zero length*/

         DEBUG_STR("shell->find(%s): NULL", program);

         return NULL;

      } else {

         /* be sure to end string by '\0' character */
         buffer[len] = '\0';

         /* we need only first line */
         for (int i = 0; i < len; i++) {
            if (buffer[i] == '\n') {
               buffer[i] = '\0';
               break;
            }
         }

         DEBUG_STR("shell->find(%s): \"%s\"", program, buffer);
      }

      /* clean child's status from process table */
      waitpid(pid, NULL, 0);

      return buffer;
   }
}

static bool
process_stop(process *p)
{
   /* kill the process, if running */
   if (p->running) {
      DEBUG_STR("process->stop(pid %d)", p->pid);

      if (p->pid) {
         kill(p->pid, 9);
         waitpid(p->pid, NULL, 0);
         p->pid = 0;
         close(p->pipe[0]);
         close(p->pipe[1]);
      } else {
         p->running = false;
      }

      return true;
   }
   return false;
}

static bool
process_read(process *p, NPVariant *result)
{
   int len;
   int status;
   NPString str;
   NPUTF8 *chars;

   /* check if the process is {still,already} running */
   if (p->running) {

      /* read from pipe */
      if ((len = read(p->pipe[0], buffer, BUFLEN - 1)) == -1) {

         if (errno == EAGAIN || errno == EWOULDBLOCK) {
            /* non-blocking reading */

            len = 0;

         } else {
            /* error */

            p->running = false;
            //p->error = errno;

         }

      } else if (len == 0) {
         /* no data - test if child became a zombie */

         waitpid(p->pid, &status, WNOHANG);

         if (WIFEXITED(status)) {
            /* child finished, clean it's status from process table */
            waitpid(p->pid, NULL, 0);
            p->running = false;
         }

      }

   } else {

      len = 0;

   }

   buffer[len] = '\0';

   /* fill the result string */
   chars = npnfuncs->memalloc((len + 1) * sizeof(NPUTF8));
   memcpy(chars, buffer, len);
   STRING_UTF8CHARACTERS(str) = chars;
   STRING_UTF8LENGTH(str) = len;

   result->type = NPVariantType_String;
   result->value.stringValue = str;

   return true;
}


static void
process_destroy(process *p)
{
   DEBUG_STR("process->destroy()");
   npnfuncs->releaseobject(p->obj);
   npnfuncs->memfree(p);
}

process *
process_init()
{
   process *p;

   if ((p = (process *)npnfuncs->memalloc(sizeof(process))) == NULL)
      return NULL;

   p->next = NULL;

   p->pid = 0;
   p->running = false;

   p->destroy = process_destroy;
   p->read = process_read;
   p->stop = process_stop;

}

static process *
run_command(const char *path, char *const argv[])
{
   process *p;

   if ((p = process_init()) == NULL)
      return NULL;

   /* create pipe for communication */
   if (pipe(p->pipe) == -1) {
      DEBUG_STR("shell->run(): pipe() error");
      npnfuncs->setexception(NULL, "shell->run(): pipe() error");
      p->destroy(p);
      return NULL;
   }

   /* fork the process */
   if ((p->pid = vfork()) == 0) {
      /* child */
      DEBUG_STR("shell->run(): child");

      /* close read end of pipe */
      close(p->pipe[0]);

      /* stdout and stderr to write end of the pipe */
      if (dup2(p->pipe[1], 1) == -1 || dup2(p->pipe[1], 2) == -1) {
         npnfuncs->setexception(NULL, "startCommand(): dup2() error");
         _exit(1);
      }

      /* execute command */
      if (execv(path, argv) == -1) {
         npnfuncs->setexception(NULL, "shell->run(): execv() error");
         _exit(1);
      }

      /* won't get here - executed program exits */

   } else if (p->pid == -1) {
      /* error - can't fork the parent process */

      DEBUG_STR("shell->run(): vfork() error");
      npnfuncs->setexception(NULL, "shell->run(): vfork() error");
      p->destroy(p);
      return NULL;

   } else {
      /* parent */
      DEBUG_STR("shell->run(\"%s ...\"): pid %d", path, p->pid);

      /* close write end of pipe */
      close(p->pipe[1]);

      /* make read end of pipe non-blocking */
      int flags;
      if ((flags = fcntl(p->pipe[0], F_GETFL)) == -1) {
         DEBUG_STR("shell->run(): fcntl(F_GETFL) error");
         npnfuncs->setexception(NULL, "shell->run(): fcntl(F_GETFL) error");
         close(p->pipe[0]);
         p->destroy(p);
         return NULL;
      }
      if (fcntl(p->pipe[0], F_SETFL, flags | O_NONBLOCK) == -1) {
         DEBUG_STR("shell->run(): fcntl(F_SETFL) error");
         npnfuncs->setexception(NULL, "shell->run(): fcntl(F_SETFL) error");
         close(p->pipe[0]);
         p->destroy(p);
         return NULL;
      }

      p->running = true;
   }

   return p;
}


static void
destroy()
{
   DEBUG_STR("shell->destroy()");

   if (shell != NULL)
      npnfuncs->memfree(shell);
   if (buffer != NULL)
      npnfuncs->memfree(buffer);
   if (which_env[0] != NULL)
      npnfuncs->memfree(which_env[0]);
}

cmd_shell *
init_shell()
{
   cmd_shell *shell;

   DEBUG_STR("shell->init()");

   /* Allocate shell object */
   if ((shell = npnfuncs->memalloc(sizeof(cmd_shell))) == NULL)
      return NULL;

   shell->destroy = destroy;
   shell->find = find_program_path;
   shell->run = run_command;

   /* Allocate buffer */
   if ((buffer = npnfuncs->memalloc(BUFLEN * sizeof(char))) == NULL)
      return NULL;

   /* Get user paths from variable PATH */
   if ((user_paths = getenv("PATH")) == NULL)
      return NULL;

   /* Allocate modified PATH variable */
   if ((which_env[0] = npnfuncs->memalloc((strlen(user_paths) + strlen(root_paths) + 1) * sizeof(char))) == NULL)
      return NULL;

   /* Add superuser paths into PATH variable:
      PATH="$PATH:/usr/sbin/:/sbin/"
   */
   memcpy(which_env[0], "PATH=", strlen("PATH="));
   memcpy(which_env[0] + strlen("PATH="), user_paths, strlen(user_paths));
   strncpy(which_env[0] + strlen("PATH=") + strlen(user_paths), root_paths, strlen(root_paths));

   return shell;
}
