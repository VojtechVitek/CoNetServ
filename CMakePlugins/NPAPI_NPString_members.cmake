include(CheckCSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES ${NPAPI_INCLUDE_DIR})

CHECK_C_SOURCE_COMPILES("
#define bool int
#include <npapi.h>
#include <nptypes.h>
#include <npruntime.h>
int main()
{
   NPString str;
   str.UTF8Characters = 0;
   str.UTF8Length = 0;
   return 0;
}
" HAVE_NPAPI_NPSTRING_CAMEL_CASE_MEMBERS)

CHECK_C_SOURCE_COMPILES("
#define bool int
#include <npapi.h>
#include <nptypes.h>
#include <npruntime.h>
int main()
{
   NPString str;
   str.utf8characters = 0;
   str.utf8length = 0;
   return 0;
}
" HAVE_NPAPI_NPSTRING_LOWER_CASE_MEMBERS)

set(CMAKE_REQUIRED_INCLUDES)

if(NOT "${HAVE_NPAPI_NPSTRING_CAMEL_CASE_MEMBERS}" STREQUAL "")
   set(HAVE_NPAPI_NPSTRING_WHAT_CASE_MEMBERS "HAVE_NPAPI_NPSTRING_CAMEL_CASE_MEMBERS")
   message(STATUS "NPAPI NPString members recognized as CAMEL CASE")
elseif(NOT "${HAVE_NPAPI_NPSTRING_LOWER_CASE_MEMBERS}" STREQUAL "")
   set(HAVE_NPAPI_NPSTRING_WHAT_CASE_MEMBERS "HAVE_NPAPI_NPSTRING_LOWER_CASE_MEMBERS")
   message(STATUS "NPAPI NPString members recognized as LOWER CASE")
else()
   message(FATAL_ERROR "NPAPI NPString members couldn't be recognized.")
endif()
