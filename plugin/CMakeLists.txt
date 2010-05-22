# Headers
set(headers
	conetserv.h
	config.h
)

# Sources
set(sources
	conetserv.c
)
set(debug_sources
	debug.c
)

# Platform-specific sources
if(UNIX)
	set(platform_sources conetserv_posix.c)
elseif(WIN32)
	set(platform_sources conetserv_win.c)
else()
	message(FATAL ERROR "Sorry, your operating system is not supported.")
endif()

find_package(XULRunnerSDK REQUIRED)
if(NOT "${XULRunnerSDK_FOUND}" STREQUAL "YES")
	message(FATAL_ERROR "Please install XULRunner SDK (xulrunner-devel>=1.9).")
endif()

message("-- XULRunner SDK: ${XULRunnerSDK_INCLUDE_DIR}")
include_directories(${XULRunnerSDK_INCLUDE_DIR})
add_definitions(-DXULRUNNER_SDK)


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

# Create library
add_library(CoNetServ SHARED ${sources} ${platform_sources} ${headers})
set_target_properties(CoNetServ PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(CoNetServ PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.0 SOVERSION 1)

# Create debug program
add_executable(debug ${debug_sources} ${headers})
target_link_libraries(debug CoNetServ)