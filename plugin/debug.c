#include <stdarg.h>
#include <stdio.h>

#include "config.h"
#include "debug.h"

#if defined(DEBUG)

void debug(const char *args, ...)
{
   FILE *file;
   va_list ap;
   va_start(ap, args);

#if defined(_WINDOWS)
   if (fopen("\\conetserv.log", "a")) {
      fprintf(file, "%s: ", PLUGIN_NAME);
      fprintf(file, args);
      fclose(file);
   }
#else
   fprintf(stderr, "%s: ", PLUGIN_NAME);
   fprintf(stderr, args);
   if (file = fopen("/tmp/conetserv.log", "a")) {
      fprintf(file, "%s: ", PLUGIN_NAME);
      fprintf(file, args);
      fclose(file);
   }
#endif

   va_end(ap);
}

#endif