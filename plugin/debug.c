#include <stdio.h>
#include <string.h>

#include "config.h"
#include "plugin.h"

char buffer[BUFFER_LENGTH];

int main(int argc, char **argv)
{
   NPUTF8 str[] = "github.com";
   command_t cmd;
   int len;

   if (argc == 2 && !strcmp(argv[1], "PING"))
      cmd = PING;
   else if (argc == 2 && !strcmp(argv[1], "PING6"))
      cmd = PING6;
   else if (argc == 2 && !strcmp(argv[1], "TRACEROUTE"))
      cmd = TRACEROUTE;
   else if (argc == 2 && !strcmp(argv[1], "TRACEROUTE6"))
      cmd = TRACEROUTE6;
   else if (argc == 2 && !strcmp(argv[1], "WHOIS"))
      cmd = WHOIS;
   else if (argc == 2 && !strcmp(argv[1], "LOCALE"))
      cmd = LOCALE;
   else {
      fprintf(stderr, "Second parameter should be a command name.\nPING\nPING6\nTRACEROUTE\nTRACEROUTE6\nWHOIS\n");
      return 1;
   }

   startCommand(cmd, str);

   for(;;) {
      if ((len = readCommand(cmd, buffer)) > 0)
         fprintf(stdout, "%s", buffer);
      else if (len == 0)
         continue;
      else
         break;
   }

   return 0;
}
