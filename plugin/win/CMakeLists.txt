# CoNetServ plugin - Windows

# File is included -> current path is ../
include_directories(win)

set(PLUGIN_FILENAME "np${PROJECT_NAME}.dll")
configure_file("win/dll_export.def.in" "${CMAKE_BINARY_DIR}/dll_export.def" IMMEDIATE @ONLY)

set(sources
	${sources}
	win/commands.c
)

set(library_sources
	${library_sources}
	${CMAKE_BINARY_DIR}/dll_export.def
	win/resource.h
	win/resource.rc
)

set(headers
	${headers}
	win/stdbool.h
)

find_package(XULRunnerSDK REQUIRED)
if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
	message(FATAL_ERROR "Set XULRunnerSDK_INCLUDE_DIR variable to path of your XULRunner SDK include directory.\nFor downloading XULRunner SDK, visit https://developer.mozilla.org/en/Gecko_SDK#Downloading")
endif()
message(STATUS "XULRunner SDK found: ${XULRunnerSDK_INCLUDE_DIR}")
include_directories(${XULRunnerSDK_INCLUDE_DIR})
add_definitions(-DXULRUNNER_SDK)
