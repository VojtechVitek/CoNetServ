#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <string.h>

#include "config.h"
#include "plugin_npapi.h"

#if defined(DEBUG)

/* Print formatted string */
#define DEBUG_STR(fmt, ...) \
   do { \
      fprintf(stderr, "%s(%16s:%03d): " fmt "\n", PLUGIN_NAME, (strlen(__FILE__) > 16 ? __FILE__ + strlen(__FILE__) - 16 : __FILE__), __LINE__, ## __VA_ARGS__); \
      while (debug_identifier_ptr) { \
         browser->memfree(debug_identifier_str[debug_identifier_ptr--]); \
      } \
   } while (0);

/* Debug identifier strings that have to be freed */
#define DEBUG_IDENTIFIER(identifier) \
   (debug_identifier_str[debug_identifier_ptr++] = \
      browser->utf8fromidentifier((identifier)))
#define DEBUG_IDENTIFIER_COUNT 10
extern NPUTF8 *debug_identifier_str[DEBUG_IDENTIFIER_COUNT];
extern int    debug_identifier_ptr;

#else

#define DEBUG_STR(fmt, ...)
#define DEBUG_IDENTIFIER(identifier)

#endif

#endif /*_DEBUG_H_*/
