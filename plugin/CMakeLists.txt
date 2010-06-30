# CoNetServ plugin

# Sources and header files
include_directories(${CMAKE_BINARY_DIR})

configure_file(config.h.in ${CMAKE_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(sources
   debug.c
	plugin_npapi.c
)

set(headers
   ${CMAKE_BINARY_DIR}/config.h
	plugin_npapi.h
   plugin_module.h
)

# Platform-specific sources and header files
if(UNIX)
	include(unix/CMakeLists.txt)
elseif(WIN32)
	include(win/CMakeLists.txt)
else()
	message(FATAL ERROR "Sorry, your operating system is not supported.")
endif()

# Create shared library
add_library(CoNetServ SHARED ${sources} ${headers})
set_target_properties(CoNetServ PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(CoNetServ PROPERTIES PREFIX "np")
set_target_properties(CoNetServ PROPERTIES VERSION ${VERSION})
set_target_properties(CoNetServ PROPERTIES SOVERSION ${MAJOR_VERSION})

# Copy shared library to debug/release directory
get_target_property(CoNetServLibFile CoNetServ LOCATION)
get_filename_component(CoNetServLibFileName ${CoNetServLibFile} NAME)

include(${CMAKE_MODULE_PATH}/MozillaTargetPlatform.cmake)

# Export release shared library (force simple packaging)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")

	set(RELEASE_PACK ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}-plugins.zip)

	# Create Mozilla-like platform path in release directory
	file(MAKE_DIRECTORY ${RELEASE_DIR}/${MOZILLA_PATH}/)

	if(APPLE)
		include(apple/CMakeLists.txt)
	endif()

	add_custom_command(
		OUTPUT  ${RELEASE_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}
		COMMAND ${CMAKE_COMMAND} -E copy
		${CoNetServLibFile} ${RELEASE_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}
		DEPENDS ${CoNetServLibFile}
		COMMENT "Copying release shared library into: ${RELEASE_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}"
	)
	add_custom_command(
		OUTPUT  ${RELEASE_PACK}
		WORKING_DIRECTORY ${RELEASE_DIR}/platform
		COMMAND zip -q -r ${RELEASE_PACK} *
		DEPENDS ${RELEASE_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}
		COMMENT "Packing release plugin binaries into: ${RELEASE_PACK}"
	)
	add_custom_target(PluginReleasePack ALL
		DEPENDS ${RELEASE_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName} ${RELEASE_PACK}
	)

else()

	# Create Mozilla-like platform path in debug directory
	file(MAKE_DIRECTORY ${DEBUG_DIR}/${MOZILLA_PATH}/)

	add_custom_command(
		OUTPUT  ${DEBUG_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}
		COMMAND ${CMAKE_COMMAND} -E copy
		${CoNetServLibFile} ${DEBUG_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}
		DEPENDS ${CoNetServLibFile}
		COMMENT "Copying debug shared library into: ${DEBUG_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}"
	)

	add_custom_target(PluginCopy ALL
		DEPENDS ${DEBUG_DIR}/${MOZILLA_PATH}/${PLATFORM_PATH}${CoNetServLibFileName}
	)

endif()
