#include <stdarg.h>
#include <stdio.h>

#include "config.h"
#include "debug.h"
#include "npapi.h"

#if defined(DEBUG)

#if defined(_WINDOWS) || defined(ANDROID)
FILE *debug_stderr_file;
#endif

NPUTF8 *debug_identifier_str[DEBUG_IDENTIFIER_COUNT] = {0};
int    debug_identifier_ptr = 0;

#endif
