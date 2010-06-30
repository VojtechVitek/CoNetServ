#include <stdio.h>

void debug(const char *msg) {
   #if !defined(NDEBUG)
   #if defined(ANDROID)
   FILE *out = fopen("/tmp/conetserv.log", "a");
   if(out) {
      fputs(msg, out);
      fclose(out);
   }
   #elif !defined(_WINDOWS)
   fputs(msg, stderr);
   #else
   FILE *out = fopen("\\conetserv.log", "a");
   if(out) {
      fputs(msg, out);
      fclose(out);
   }
   #endif
   #endif
}
