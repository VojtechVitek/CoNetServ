# CoNetServ plugin - Apple (Darwin)

# NOTE: File is using special variables from ../CMakeLists.txt
# File is included -> current path is ../

# Make MacOS bundle plugin directory
set(PLATFORM_PATH np${PROJECT_NAME}.plugin/Contents/MacOS)
file(MAKE_DIRECTORY ${RELEASE_DIR}/${MOZILLA_PATH}np${PROJECT_NAME}.plugin/Contents/Resources/English.lproj)
file(MAKE_DIRECTORY ${RELEASE_DIR}/${MOZILLA_PATH}np${PROJECT_NAME}.plugin/Contents/MacOS)
configure_file("apple/Info.plist.in" "${RELEASE_PATH}/${MOZILLA_PATH}np${PROJECT_NAME}.plugin/Contents/Info.plist")

configure_file("apple/Localized.r.in" "${CMAKE_BINARY_DIR}/Localized.r")
find_program(APPLE_RESOURCE Rez /Developer/Tools)
if(APPLE_RESOURCE)
	add_custom_command(COMMAND ${APPLE_RESOURCE}
		-o ${RELEASE_PATH}/${MOZILLA_PATH}np${PROJECT_NAME}.plugin/Contents/Resources/English.lproj/Localized.rsrc
		-useDF ${CMAKE_BINARY_DIR}/Localized.r)
else
	message(FATAL_ERROR "Can't find /Developer/Tools/Rez.")
endif()

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
