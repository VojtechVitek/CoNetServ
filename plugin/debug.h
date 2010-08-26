#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <string.h>

#include "config.h"
#include "npapi.h"

#if defined(DEBUG)

#if defined(_WINDOWS) || defined(ANDROID)

#if defined(_WINDOWS)
#define DEBUG_STDERR_FILEPATH "\\conetserv.log"
#else
#define DEBUG_STDERR_FILEPATH "/tmp/conetserv.log"
#endif

extern FILE *debug_stderr_file;

/* Print debug messages to file on Windows and on Android */
#define DEBUG_STR_PLATFORM(fmt, ...) \
   do { \
      debug_stderr_file = fopen(DEBUG_STDERR_FILEPATH, "a"); \
      if (debug_stderr_file) { \
         fprintf(debug_stderr_file, fmt, ## __VA_ARGS__); \
         fclose(debug_stderr_file); \
      } \
   } while (0)

#else

/* Print debug messages to stderr on other (Unix-like) systems */
#define DEBUG_STR_PLATFORM(fmt, ...) \
   fprintf(stderr, fmt, ## __VA_ARGS__)

#endif

/* Debug formatted string */
#define DEBUG_STR(fmt, ...) \
   do { \
      DEBUG_STR_PLATFORM("%s(%16s:%03d): " fmt "\n", PLUGIN_NAME, (strlen(__FILE__) > 16 ? __FILE__ + strlen(__FILE__) - 16 : __FILE__), __LINE__, ## __VA_ARGS__); \
      while (debug_identifier_ptr) { \
         browser->memfree(debug_identifier_str[--debug_identifier_ptr]); \
      } \
   } while (0)

/* Debug identifier strings that should be freed immediately */
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
