#include <stdarg.h>
#include <stdio.h>

#include "config.h"
#include "debug.h"
#include "npapi.h"

#if defined(DEBUG)

NPUTF8 *debug_identifier_str[DEBUG_IDENTIFIER_COUNT] = {0};
int    debug_identifier_ptr = 0;

#endif
