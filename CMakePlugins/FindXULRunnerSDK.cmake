# Locate XULRunner SDK (development headers)
# This module defines:
# XULRunnerSDK_FOUND ("YES" or not defined)
# XULRunnerSDK_INCLUDE_DIR (path to find the headers)

# If the path is not explicitly defined, try to find it manually
if(NOT XULRunnerSDK_INCLUDE_DIR)

	file(GLOB path1 "/usr/include/xulrunner-*")

	find_path(XULRunnerSDK_INCLUDE_DIR npapi.h
		${path1}
		${path1}/stable
		${path1}/unstable
	)

endif()

# Test header file and set final variable if found
if(XULRunnerSDK_INCLUDE_DIR AND EXISTS "${XULRunnerSDK_INCLUDE_DIR}/npapi.h")
   set(NPAPI_INCLUDE_DIR ${XULRunnerSDK_INCLUDE_DIR})
	set(XULRunnerSDK_FOUND "YES")
else()
	message(FATAL_ERROR "Be sure to specify ABSOLUTE path of INCLUDE directory of XULRunnerSDK.")
endif()
