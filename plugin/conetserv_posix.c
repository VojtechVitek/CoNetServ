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
   {"/bin/ping", NULL, "-c", "10", NULL},
   {"/usr/sbin/traceroute", NULL, NULL},
   {"/usr/bin/whois", NULL, NULL}
};

bool startCommand(command_t cmd, char* addr)
{
   /* already started */
   if (pids[cmd] != 0)
      return false;

   /* create pipe for communication */
   if (pipe(pipes[cmd]) == -1) {
      logmsg("CoNetServ: startCommand(): pipe() - error\n");
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): pipe() - error\n");
      return false;
   }

   /* fork the subprocess */
   if ((pids[cmd] = vfork()) == 0) {
      /* child */
      logmsg("CoNetServ: startCommand(): vfork() - child\n");

      /* close read end of pipe */
      close(pipes[cmd][0]);

      /* stdout to write end of the pipe */
      if (dup2(pipes[cmd][1], 1) == -1) {
         logmsg("CoNetServ: startCommand(): dup2() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): dup2() - error\n");
         _exit(1);
      }

      /* copy addr argument to array */
      args[cmd][1] = addr;

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
