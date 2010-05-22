# Locate XULRunner SDK (development headers)
# This module defines:
# XULRunnerSDK_FOUND ("YES" or not defined)
# XULRunnerSDK_INCLUDE_DIR (path to find the headers)

if(NOT XULRunnerSDK_INCLUDE_DIR)

	file(GLOB path1 "/usr/include/xulrunner-sdk-*")

	find_path(XULRunnerSDK_INCLUDE_DIR npapi.h
		${path1}
		${path1}/stable
		${path1}/unstable
	)

endif()

if(XULRunnerSDK_INCLUDE_DIR)
	set(XULRunnerSDK_FOUND "YES")
endif()
