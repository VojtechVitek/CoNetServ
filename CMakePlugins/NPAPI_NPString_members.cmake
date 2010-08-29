include(CheckCSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES ${NPAPI_INCLUDE_DIR})

CHECK_C_SOURCE_COMPILES("
#include <npapi.h>
#include <nptypes.h>
#include <npruntime.h>
int main()
{
   NPString str = {
      .UTF8Characters = 0,
      .UTF8Length = 0
   };
   return 0;
}
" HAVE_NPAPI_NPSTRING_CAMEL_CASE_MEMBERS)

CHECK_C_SOURCE_COMPILES("
#include <npapi.h>
#include <nptypes.h>
#include <npruntime.h>
int main()
{
   NPString str = {
      .utf8characters = 0,
      .utf8length = 0
   };
   return 0;
}
" HAVE_NPAPI_NPSTRING_LOWER_CASE_MEMBERS)

set(CMAKE_REQUIRED_FLAGS)