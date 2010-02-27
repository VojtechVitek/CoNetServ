#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "conetserv.h"

pid_t cmd_pid[command_t_count] = {0};
int cmd_pipe[command_t_count][2];

char* cmd_args[command_t_count][5] = {
   {"/bin/ping", NULL, "-c", "10", NULL},
   {"/usr/sbin/traceroute", NULL, NULL},
   {"/usr/bin/whois", NULL, NULL}
};

bool startCommand(command_t cmd, char* addr)
{
   /* already started */
   if (cmd_pid[cmd] != 0)
      return false;

   /* create pipe for communication */
   if(pipe(cmd_pipe[cmd]) == -1) {
      logmsg("CoNetServ: startCommand(): pipe() - error\n");
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): pipe() - error\n");
      return false;
   }

   /* fork the subprocess */
   if((cmd_pid[cmd] = vfork()) == 0) {
      /* child */
      logmsg("CoNetServ: startCommand(): vfork() - child\n");

      /* close read end of pipe */
      close(cmd_pipe[cmd][0]);

      /* stdout to write end of the pipe */
      if (dup2(cmd_pipe[cmd][1], 1) == -1) {
         logmsg("CoNetServ: startCommand(): dup2() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): dup2() - error\n");
         _exit(1);
      }

      /* copy addr argument to array */
      cmd_args[cmd][1] = addr;

      /* execute command */
      if (execv(cmd_args[cmd][0], cmd_args[cmd]) == -1) {
         logmsg("CoNetServ: startCommand(): execv() - error\n");
         npnfuncs->setexception(NULL, "CoNetServ: startCommand(): execv() - error\n");
         //send a signal to parrent
         _exit(1);
      }
   } else if (cmd_pid[cmd] == -1) {
      /* error - can't fork the parent process */
      logmsg("CoNetServ: startCommand(): vfork() - error\n");

      cmd_pid[cmd] = 0;
      npnfuncs->setexception(NULL, "CoNetServ: startCommand(): vfork() - error\n");
      return false;
   } else {
      /* parent */
      /* close read end of pipe */
      close(cmd_pipe[cmd][1]);
      logmsg("CoNetServ: startCommand(): vfork() - parent\n");
   }
   return true;
}

bool stopCommand(command_t cmd)
{
   logmsg("CoNetServ: stopCommand()\n");

   /* kill the command, if running */
   if (cmd_pid[cmd] != 0) {
      kill(cmd_pid[cmd], 9);
      cmd_pid[cmd] = 0;
      close(cmd_pipe[cmd][0]);
      close(cmd_pipe[cmd][1]);
      return true;
   } else {
      return false;
   }
}

int readCommand(command_t cmd, char *buf)
{
   logmsg("CoNetServ: readCommand(");
   fprintf(stderr, "%d) = ", cmd);

   int len = 0;

   /* check if the process is started */
   if (cmd_pid[cmd] != 0) {
      /* read from command pipe */
      if ((len = read(cmd_pipe[cmd][0], buf, BUFFER_LENGTH - 1)) == -1) {
         /* error */
         npnfuncs->setexception(NULL, "CoNetServ: readCommand(): can't read the command pipe\n");
         len = 0;
      }
   } else {
      /* commnad not running */
      npnfuncs->setexception(NULL, "CoNetServ: readCommand(): command not running\n");
   }

   buf[len] = '\0';

   fprintf(stderr, "%d\n", len + 1);

   return len;
}
