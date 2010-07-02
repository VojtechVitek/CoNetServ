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

   DEBUG_STR("CoNetServ: find_program_path(%s)", DEBUG_GET_IDENTIFIER_1(program));
   DEBUG_FREE_IDENTIFIER_1()

   /* create pipe for communication */
   if (pipe(pipes) == -1)
      return NULL;

   /* fork the process */
   if ((pid = vfork()) == 0) {
      /* child */

      /* close read end of pipe */
      close(pipes[0]);

      /* redirect stdout to write end of the pipe */
      if (dup2(pipes[1], 1) == -1)
         _exit(1);

      /* Fill which argument by program name */
      which_argv[1] = (char *)program;

      /* Execute command
       * which <cmd>
       */
      if (execve(which_argv[0], which_argv, which_env) == -1)
         _exit(1);

   } else if (pid == -1) {
      /* error - can't fork the parent process */

      return NULL;

   } else {
      /* parent */

      /* close write end of pipe */
      close(pipes[1]);

      /* read child data from the pipe */
      if ((len = read(pipes[0], buffer, BUFLEN - 1)) == -1 || len == 0) {
         /* Error or zero length*/

         return NULL;

      } else {

         /* Be sure to end string by '\0' character */
         buffer[len] = '\0';
         /* We need only first line */
         for (int i = 0; i < len; i++) {
            if (buffer[i] == '\n') {
               buffer[i] = '\0';
               break;
            }
         }
         DEBUG_STR("find_program_path(%s): \"%s\"", DEBUG_GET_IDENTIFIER_1(buffer));
         DEBUG_FREE_IDENTIFIER_1()
      }

      waitpid(pid, NULL, 0);

      return buffer;
   }
}

static void run_command(char *program_path)
{

}

#if 0
static bool startCommand()
{
   /* already started */
   if (pids[cmd] != 0)
      return false;

   /* workaround for execvp() */
   execvp_workaround();

   /* create pipe for communication */
   if (pipe(pipes[cmd]) == -1) {
      DEBUG_STR("CoNetServ: startCommand(): pipe() - error\n");
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): pipe() - error\n");
      return false;
   }

   /* fork the process */
   if ((pids[cmd] = vfork()) == 0) {
      /* child */
      DEBUG_STR("CoNetServ: startCommand(): vfork() - child\n");

      /* close read end of pipe */
      close(pipes[cmd][0]);

      /* stdout and stderr to write end of the pipe */
      if (dup2(pipes[cmd][1], 1) == -1 || dup2(pipes[cmd][1], 2) == -1) {
         DEBUG_STR("CoNetServ: startCommand(): dup2() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): dup2() - error\n");
         _exit(1);
      }

      /* copy addr argument to array */
      args[cmd][1] = arg_host;

      /* execute command */
      if (execv(args[cmd][0], args[cmd]) == -1) {
         DEBUG_STR("CoNetServ: startCommand(): execv() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): execv() - error\n");
         //send a signal to parrent
         _exit(1);
      }
   } else if (pids[cmd] == -1) {
      /* error - can't fork the parent process */
      DEBUG_STR("CoNetServ: startCommand(): vfork() - error\n");

      pids[cmd] = 0;
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): vfork() - error\n");
      return false;
   } else {
      /* parent */
      DEBUG_STR("CoNetServ: startCommand(): vfork() - parent\n");

      /* close write end of pipe */
      close(pipes[cmd][1]);

      /* make read end of pipe non-blocking */
      int flags;
      if ((flags = fcntl(pipes[cmd][0], F_GETFL)) == -1) {
         DEBUG_STR("CoNetServ: startCommand(): fcntl(F_GETFL) - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): fcntl(F_GETFL) - error\n");
         return false;
      }
      if (fcntl(pipes[cmd][0], F_SETFL, flags | O_NONBLOCK) == -1) {
         DEBUG_STR("CoNetServ: startCommand(): fcntl(F_SETFL) - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): fcntl(F_SETFL) - error\n");
         return false;
      }
   }
   return true;
}

static bool stopCommand(command_t cmd)
{
   /* kill the command, if running */
   if (pids[cmd] != 0) {
      DEBUG_STR("CoNetServ: stopCommand()\n");
      kill(pids[cmd], 9);
      waitpid(pids[cmd], NULL, 0);
      pids[cmd] = 0;
      close(pipes[cmd][0]);
      close(pipes[cmd][1]);
      return true;
   } else {
      return false;
   }
}

static int readCommand(command_t cmd, char *buf)
{
   int len;

   /* check if the process is started */
   if (pids[cmd] != 0) {
      /* read from command pipe */
      if ((len = read(pipes[cmd][0], buf, BUFFER_LENGTH - 1)) == -1) {
         /* error but no data while non-blocking reading */
         if (errno == EAGAIN || errno == EWOULDBLOCK)
            len = 0;
      } else if (len == 0) {
         /* no data - child possibly became a zombie */
         int status;
         waitpid(pids[cmd], &status, WNOHANG);
         if (WIFEXITED(status)) {
            /* child was really exited, clean it's status from process table */
            waitpid(pids[cmd], NULL, 0);
            pids[cmd] = 0;
         }
      }
   } else {
      /* commnad not running */
      return -1;
   }

   buf[len] = '\0';

   if (len != 0) {
      DEBUG_STR("CoNetServ: readCommand()");
      //fprintf(stderr, "cmd = %d), len = %d\n", cmd, len);
      //fprintf(stderr, "%s", buf);
   }

   return len;
}
#endif

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

   DEBUG_STR("init_shell()");

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

                                                                                                   /* Allocate buffer */
   if ((shell = npnfuncs->memalloc(sizeof(cmd_shell))) == NULL)
      return NULL;

   shell->destroy = destroy;
   shell->find = find_program_path;
   shell->run = run_command;

   return shell;
}
