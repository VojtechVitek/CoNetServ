# CoNetServ plugin - Windows

# File is included -> current path is ../
configure_file("win/dll_export.def.in" "${CMAKE_BINARY_DIR}/win/dll_export.def")
include_directories(win/ ${CMAKE_BINARY_DIR}/win)
set(platform_sources
	win/commands.c
)
set(platform_headers
	win/config.h
	win/stdbool.h
	${CMAKE_BINARY_DIR}/win/dll_export.def
)

find_package(XULRunnerSDK REQUIRED)
if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
	message(FATAL_ERROR "Set XULRunnerSDK_INCLUDE_DIR variable to path of your XULRunner SDK include directory.\nFor downloading XULRunner SDK, visit https://developer.mozilla.org/en/Gecko_SDK#Downloading")
endif()
message(STATUS "XULRunner SDK found: ${XULRunnerSDK_INCLUDE_DIR}")
include_directories(${XULRunnerSDK_INCLUDE_DIR})
add_definitions(-DXULRUNNER_SDK)
