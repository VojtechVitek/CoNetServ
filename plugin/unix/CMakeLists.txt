# CoNetServ plugin - Unix-like systems

# File is included -> current path is ../
include_directories(unix)

set(sources
	${sources}
	unix/shell.c
	unix/ping.c
	unix/traceroute.c
	unix/nslookup.c
	unix/whois.c
	unix/dig.c
)

set(headers
	${headers}
	unix/init_modules.h
	unix/process.h
	unix/shell.h
)

add_definitions(-std=gnu99)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_definitions(-O2)
else()
	add_definitions(-g)
endif()

find_package(XULRunnerSDK REQUIRED)
if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
	message(FATAL_ERROR "Please install XULRunner SDK (xulrunner-devel>=1.9).")
endif()
message(STATUS "XULRunner SDK found: ${XULRunnerSDK_INCLUDE_DIR}")
include_directories(${XULRunnerSDK_INCLUDE_DIR})
add_definitions(-DXULRUNNER_SDK)
