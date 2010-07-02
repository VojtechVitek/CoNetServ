#include <stdarg.h>
#include <stdio.h>

#include "plugin_npapi.h"
#include "config.h"
#include "debug.h"

#if defined(DEBUG)

NPUTF8 *debug_identifier_str[DEBUG_IDENTIFIER_COUNT] = {0};
int    debug_identifier_ptr = 0;

void DEBUG_STR(const char *fmt, ...)
{
   int i;
   FILE *file;
   va_list va;
   va_start(va, fmt);

#if defined(_WINDOWS)
   if (fopen("\\" PLUGIN_NAME ".log", "a")) {
      fprintf(file, "%s: ", PLUGIN_NAME);
      vfprintf(file, fmt, va);
      fprintf(file, "\n");
      fclose(file);
   }
#else
   fprintf(stderr, "%s: ", PLUGIN_NAME);
   vfprintf(stderr, fmt, va);
   fprintf(stderr, "\n");
   if (file = fopen("/tmp/" PLUGIN_NAME ".log", "a")) {
      fprintf(file, "%s: ", PLUGIN_NAME);
      vfprintf(file, fmt, va);
      fprintf(file, "\n");
      fclose(file);
   }
#endif

   while (debug_identifier_ptr) {
      npnfuncs->memfree(debug_identifier_str[debug_identifier_ptr--]);
   }

   va_end(va);
}

#endif