#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "plugin_npapi.h"

#if defined(DEBUG)

/* Print formatted string */
void DEBUG_STR(const char *fmt, ...);

/* Debug identifier strings, that have to be freed */
extern NPUTF8 *str1, *str2, *str3, *str4, *str5;
#define DEBUG_GET_IDENTIFIER_1(identifier) (str1 = npnfuncs->utf8fromidentifier((identifier)))
#define DEBUG_GET_IDENTIFIER_2(identifier) (str2 = npnfuncs->utf8fromidentifier((identifier)))
#define DEBUG_GET_IDENTIFIER_3(identifier) (str3 = npnfuncs->utf8fromidentifier((identifier)))
#define DEBUG_GET_IDENTIFIER_4(identifier) (str4 = npnfuncs->utf8fromidentifier((identifier)))
#define DEBUG_GET_IDENTIFIER_5(identifier) (str5 = npnfuncs->utf8fromidentifier((identifier)))
#define DEBUG_FREE_IDENTIFIER_1() npnfuncs->memfree(str1);
#define DEBUG_FREE_IDENTIFIER_2() npnfuncs->memfree(str2);
#define DEBUG_FREE_IDENTIFIER_3() npnfuncs->memfree(str3);
#define DEBUG_FREE_IDENTIFIER_4() npnfuncs->memfree(str4);
#define DEBUG_FREE_IDENTIFIER_5() npnfuncs->memfree(str5);

#else

#define DEBUG_STR(fmt, ...)
#define DEBUG_CODE(code)

#endif

#endif /*_DEBUG_H_*/
