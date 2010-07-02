#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "plugin_npapi.h"

#if defined(DEBUG)

/* Print formatted string */
void DEBUG_STR(const char *fmt, ...);

/* Debug identifier strings that have to be freed */
#define DEBUG_IDENTIFIER(identifier) (debug_identifier_str[debug_identifier_ptr++] = npnfuncs->utf8fromidentifier((identifier)))
#define DEBUG_IDENTIFIER_COUNT 10
extern NPUTF8 *debug_identifier_str[DEBUG_IDENTIFIER_COUNT];
extern int    debug_identifier_ptr;

#else

#define DEBUG_STR(fmt, ...)
#define DEBUG_CODE(code)

#endif

#endif /*_DEBUG_H_*/
