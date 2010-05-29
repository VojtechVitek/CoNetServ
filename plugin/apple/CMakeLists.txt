# CoNetServ plugin - Apple (Darwin)

# File is included -> current path is ../
configure_file("apple/Info.plist.in" "${CMAKE_BINARY_DIR}/plugin/apple/Info.plist")
configure_file("apple/Localized.r.in" "${CMAKE_BINARY_DIR}/plugin/apple/Localized.r")

# TODO:
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
