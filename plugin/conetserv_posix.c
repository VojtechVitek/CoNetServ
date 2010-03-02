#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "conetserv.h"

pid_t pids[command_t_count] = {0};

int pipes[command_t_count][2];

char* args[command_t_count][5] = {
   {"ping", NULL},
   {"ping6", NULL},
   {"traceroute", NULL},
   {"traceroute6", NULL},
   {"whois", NULL}
};

/*

This should work with GLibC 2.11+, where is execvpe() implemented.
But now it's a time for this work-around (as for Feb 2010):
$ PATH="$PATH:/usr/sbin/:/sbin/" which traceroute
-> get char* path -> execv(path, ..
-- V-Teq

char* env[] = { "PATH=$PATH:/usr/sbin:/sbin/", NULL };

args[cmd][1] = addr;
execvpe(args[cmd][0], args[cmd], env);

*/

/*
   These paths are default and will not work on all UNIX systems.
   They will be rewritten by function execvp_workaround()
   if possible to find better path.
*/
char execvp_workaround_paths[command_t_count][30] = {
   "/bin/ping",
   "/bin/ping6",
   "/usr/sbin/traceroute",
   "/usr/sbin/traceroute6",
   "/usr/bin/whois"
};
#include <string.h>
void execvp_workaround()
{
   /* run this function only once */
   static bool run = false;
   if (run)
      return;
   else
      run = true;

   fprintf(stderr, "CoNetServ: execvp_workaround()\n");

   int pipes[2];
   int pids;

   char buffer[BUFFER_LENGTH];
   char path[BUFFER_LENGTH];
   char *argv[] = { "/usr/bin/which", NULL, NULL };
   char *user_paths = getenv("PATH");
   char *superuser_paths = ":/usr/sbin:/sbin/";
   char *env[] = { path, NULL };

   memcpy(path, "PATH=", strlen("PATH="));
   memcpy(path + strlen("PATH="), user_paths, strlen(user_paths));
   strncpy(path + strlen("PATH=") + strlen(user_paths), superuser_paths, strlen(superuser_paths));

   for (int i = 0; i < command_t_count; ++i) {
      /* create pipe for communication */
      if (pipe(pipes) == -1)
         return;

      /* fork the process */
      if ((pids = vfork()) == 0) {
         /* child */

         /* close read end of pipe */
         close(pipes[0]);

         /* stdout to write end of the pipe */
         if (dup2(pipes[1], 1) == -1)
            _exit(1);

         argv[1] = args[i][0];

         /* execute command */
         if (execve(argv[0], argv, env) == -1)
            _exit(1);

      } else if (pids == -1) {
         /* error - can't fork the parent process */

         continue;

      } else {
         /* parent */

         /* close read end of pipe */
         close(pipes[1]);

         /* read child data */
         int len;
         if ((len = read(pipes[0], buffer, BUFFER_LENGTH - 1)) == -1 || len == 0) {

            continue;

         } else {

            buffer[len - 1] = '\0';
            fprintf(stderr, "CoNetServ: execvp_workaround(): read %d bytes: \"%s\"\n", len, buffer);

            /* store new command name from which command */
            strncpy(execvp_workaround_paths[i], buffer, len);
         }

         waitpid(pids, NULL, 0);
      }

      args[i][0] = execvp_workaround_paths[i];

      fprintf(stderr, "CoNetServ: execvp_workaround(): chosen: %s\n", args[i][0]);

   }
}

bool startCommand(command_t cmd, NPString addr)
{
   /* already started */
   if (pids[cmd] != 0)
      return false;

   /* workaround for execvp() */
   execvp_workaround();

   /* create pipe for communication */
   if (pipe(pipes[cmd]) == -1) {
      logmsg("CoNetServ: startCommand(): pipe() - error\n");
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): pipe() - error\n");
      return false;
   }

   /* fork the process */
   if ((pids[cmd] = vfork()) == 0) {
      /* child */
      logmsg("CoNetServ: startCommand(): vfork() - child\n");

      /* close read end of pipe */
      close(pipes[cmd][0]);

      /* stdout and stderr to write end of the pipe */
      if (dup2(pipes[cmd][1], 1) == -1 || dup2(pipes[cmd][1], 2) == -1) {
         logmsg("CoNetServ: startCommand(): dup2() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): dup2() - error\n");
         _exit(1);
      }

      /* copy addr argument to array */
      args[cmd][1] = (char *)STRING_UTF8CHARACTERS(addr);

      /* execute command */
      if (execv(args[cmd][0], args[cmd]) == -1) {
         logmsg("CoNetServ: startCommand(): execv() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): execv() - error\n");
         //send a signal to parrent
         _exit(1);
      }
   } else if (pids[cmd] == -1) {
      /* error - can't fork the parent process */
      logmsg("CoNetServ: startCommand(): vfork() - error\n");

      pids[cmd] = 0;
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): vfork() - error\n");
      return false;
   } else {
      /* parent */
      logmsg("CoNetServ: startCommand(): vfork() - parent\n");

      /* close read end of pipe */
      close(pipes[cmd][1]);

      /* make read end of pipe non-blocking */
      int flags;
      if ((flags = fcntl(pipes[cmd][0], F_GETFL)) == -1) {
         logmsg("CoNetServ: startCommand(): fcntl(F_GETFL) - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): fcntl(F_GETFL) - error\n");
         return false;
      }
      if (fcntl(pipes[cmd][0], F_SETFL, flags | O_NONBLOCK) == -1) {
         logmsg("CoNetServ: startCommand(): fcntl(F_SETFL) - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): fcntl(F_SETFL) - error\n");
         return false;
      }
   }
   return true;
}

bool stopCommand(command_t cmd)
{
   logmsg("CoNetServ: stopCommand()\n");

   /* kill the command, if running */
   if (pids[cmd] != 0) {
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

int readCommand(command_t cmd, char *buf)
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

   logmsg("CoNetServ: readCommand(");
   fprintf(stderr, "%d), len = %d\n", cmd, len);

   return len;
}
