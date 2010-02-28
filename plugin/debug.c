#include <stdio.h>
#include <string.h>

#include "conetserv.h"

char buffer[BUFFER_LENGTH];

int main(int argc, char **argv)
{
   command_t cmd;
   int len;

   if (argc == 2 && !strcmp(argv[1], "TRACEROUTE"))
      cmd = TRACEROUTE;
   else if (argc == 2 && !strcmp(argv[1], "WHOIS"))
      cmd = WHOIS;
   else
      cmd = PING;
   startCommand(cmd, "fres-solutions.com");

   for(;;) {
      if ((len = readCommand(cmd, buffer)) > 0)
         printf("%s", buffer);
      else if (len == 0)
         continue;
      else
         break;
   }

   return 0;
}
