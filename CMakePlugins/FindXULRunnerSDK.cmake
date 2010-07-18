# Locate XULRunner SDK (development headers)
# This module defines:
# XULRunnerSDK_FOUND ("YES" or not defined)
# XULRunnerSDK_INCLUDE_DIR (path to find the headers)

if(NOT XULRunnerSDK_INCLUDE_DIR)

	file(GLOB path1 "/usr/include/xulrunner-*")

	find_path(XULRunnerSDK_INCLUDE_DIR npapi.h
		${path1}
		${path1}/stable
		${path1}/unstable
	)

endif()

if(XULRunnerSDK_INCLUDE_DIR AND EXISTS "${XULRunnerSDK_INCLUDE_DIR}/npapi.h")
	set(XULRunnerSDK_FOUND "YES")
else()
	message(FATAL_ERROR "Be sure to specify INCLUDE directory of XULRunnerSDK.")
endif()
