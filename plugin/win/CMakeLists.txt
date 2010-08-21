# CoNetServ plugin - Windows

# File is included -> current path is ../
include_directories(win)

set(PLUGIN_FILENAME "np${PROJECT_NAME}.dll")
configure_file("win/dll_export.def.in" "${CMAKE_BINARY_DIR}/dll_export.def" IMMEDIATE @ONLY)
configure_file("win/resource.rc.in" "${CMAKE_BINARY_DIR}/resource.rc" IMMEDIATE @ONLY)

set(sources
	${sources}
   ${CMAKE_BINARY_DIR}/dll_export.def
   ${CMAKE_BINARY_DIR}/resource.rc
   win/commands.c
)

set(headers
	${headers}
	win/stdbool.h
   win/resource.h
)

find_package(XULRunnerSDK REQUIRED)
if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
	message(FATAL_ERROR "Set XULRunnerSDK_INCLUDE_DIR variable to path of your XULRunner SDK include directory.\nFor downloading XULRunner SDK, visit https://developer.mozilla.org/en/Gecko_SDK#Downloading")
endif()
message(STATUS "XULRunner SDK found: ${XULRunnerSDK_INCLUDE_DIR}")
include_directories(${XULRunnerSDK_INCLUDE_DIR})
add_definitions(-DXULRUNNER_SDK)
