# CoNetServ plugin

# Sources and header files
include_directories(".")
configure_file("config.h.in" "${CMAKE_BINARY_DIR}/config.h")
include_directories(${CMAKE_BINARY_DIR})
set(sources
	plugin.c
)
set(debug_sources
	debug.c
)
set(headers
	plugin.h
	${CMAKE_BINARY_DIR}/config.h
)

# Platform-specific sources and header files
if(APPLE)
	configure_file("apple/Info.plist.in" "${CMAKE_BINARY_DIR}/Info.plist")
	configure_file("apple/Localized.r.in" "${CMAKE_BINARY_DIR}/Localized.r")
	include_directories("unix")
	set(platform_sources
		unix/commands.c
	)
elseif(UNIX)
	include_directories("unix")
	set(platform_sources
		unix/commands.c
	)
elseif(WIN32)
	include_directories("win")
	configure_file("win/dll_export.def.in" "${CMAKE_BINARY_DIR}/dll_export.def")
	set(platform_sources
		win/commands.c
	)
	set(platform_headers
		win/config.h
		win/stdbool.h
		${CMAKE_BINARY_DIR}/dll_export.def
	)
else()
	message(FATAL ERROR "Sorry, your operating system is not supported.")
endif()

# Platform-specific settings
if(UNIX)
	add_definitions(-std=gnu99)
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		add_definitions(-O2)
	else()
		add_definitions(-g)
	endif()
endif()

# Platform-specific libraries and development files
if(APPLE)

	find_package(WebKitDarwinSDK REQUIRED)
	# ifeq (${KERNEL},Darwin)
	# OSCFLAGS = -DWEBKIT_DARWIN_SDK
	# OSLDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
	# else
	# OSINCS = -I/usr/include/xulrunner-1.9.1.8/stable/
	# OSCFLAGS = -DXULRUNNER_SDK
	# OSLDFLAGS = -lc
	# endif

	#MACOS RESOURCES
	#  ADD_EXECUTABLE(MyExecutable ${MyExecutable_SRCS})
	#  GET_TARGET_PROPERTY(MyExecutable_PATH MyExecutable LOCATION)
	# 
	#  IF(APPLE)
	#    FIND_PROGRAM(APPLE_RESOURCE Rez /Developer/Tools)
	#    IF(APPLE_RESOURCE)
	#      ADD_CUSTOM_COMMAND(TARGET MyExecutable POST_BUILD
	#                         COMMAND ${APPLE_RESOURCE} Carbon.r -o ${MyExecutable_PATH})
	#    ENDIF(APPLE_RESOURCE)
	#  ENDIF(APPLE)
	#CMAKE_OSX_ARCHITECTURES=ppc;i386;ppc64;x86_64

elseif(UNIX)

	find_package(XULRunnerSDK REQUIRED)
	if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
		message(FATAL_ERROR "Please install XULRunner SDK (xulrunner-devel>=1.9).")
	endif()
	message(STATUS "XULRunner SDK found: ${XULRunnerSDK_INCLUDE_DIR}")
	include_directories(${XULRunnerSDK_INCLUDE_DIR})
	add_definitions(-DXULRUNNER_SDK)

elseif(WIN32)

	find_package(XULRunnerSDK REQUIRED)
	if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
		message(FATAL_ERROR "Set XULRunnerSDK_INCLUDE_DIR variable to path of your XULRunner SDK include directory.\nFor downloading XULRunner SDK, visit https://developer.mozilla.org/en/Gecko_SDK#Downloading")
	endif()
	message(STATUS "XULRunner SDK found: ${XULRunnerSDK_INCLUDE_DIR}")
	include_directories(${XULRunnerSDK_INCLUDE_DIR})
	add_definitions(-DXULRUNNER_SDK)

endif()

# Create library (shared)
add_library(CoNetServ SHARED ${sources} ${platform_sources} ${headers} ${platform_headers})
set_target_properties(CoNetServ PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(CoNetServ PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.0 SOVERSION 1)
set_target_properties(CoNetServ PROPERTIES PREFIX "np")

#get_target_property(VAR_LOCATION CoNetServ LOCATION)
#message(STATUS ${VAR_LOCATION})

# Create debug program (static)
add_executable(debug ${debug_sources} ${sources} ${platform_sources} ${headers} ${platform_headers})