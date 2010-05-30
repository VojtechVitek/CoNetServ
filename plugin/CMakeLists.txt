# CoNetServ plugin

# Sources and header files
configure_file(config.h.in ${CMAKE_BINARY_DIR}/plugin/config.h)
include_directories(. ${CMAKE_BINARY_DIR}/plugin)
set(sources
	plugin.c
)
set(debug_sources
	debug.c
)
set(headers
	plugin.h
	${CMAKE_BINARY_DIR}/plugin/config.h
)

# Platform-specific sources and header files
if(UNIX)
	include(unix/CMakeLists.txt)
elseif(WIN32)
	include(win/CMakeLists.txt)
else()
	message(FATAL ERROR "Sorry, your operating system is not supported.")
endif()

set(CMAKE_VERBOSE_MAKEFILE true)

# Create shared library
add_library(CoNetServ SHARED ${sources} ${platform_sources} ${headers} ${platform_headers})
set_target_properties(CoNetServ PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(CoNetServ PROPERTIES PREFIX "np")

# Export release shared library (force simple packaging)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	get_target_property(CoNetServLibFile CoNetServ LOCATION)
	get_filename_component(CoNetServLibFileName ${CoNetServLibFile} NAME)

	include(${CMAKE_MODULE_PATH}/MozillaTargetPlatform.cmake)
	get_filename_component(RELEASE_DIR ${CMAKE_BINARY_DIR}/../ ABSOLUTE)
	set(RELEASE_PACK ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}-plugin-binaries.tgz)

	file(MAKE_DIRECTORY ${RELEASE_DIR}/${MOZILLA_PATH})

	if(APPLE)
		include(apple/CMakeLists.txt)
	endif()

	add_custom_command(
		OUTPUT  ${RELEASE_DIR}/${MOZILLA_PATH}${PLATFORM_PATH}${CoNetServLibFileName}
		COMMAND ${CMAKE_COMMAND} -E copy
		${CoNetServLibFile} ${RELEASE_DIR}/${MOZILLA_PATH}${PLATFORM_PATH}${CoNetServLibFileName}
		DEPENDS ${CoNetServLibFile}
		COMMENT "Copying release shared library into: ../${MOZILLA_PATH}${PLATFORM_PATH}${CoNetServLibFileName}"
	)
	add_custom_command(
		OUTPUT  ${RELEASE_PACK}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../
		COMMAND ${CMAKE_COMMAND} -E tar -czf ${RELEASE_PACK} platform
		DEPENDS ${RELEASE_DIR}/${MOZILLA_PATH}${PLATFORM_PATH}${CoNetServLibFileName}
		COMMENT "Packing release plugin binaries into: ${RELEASE_PACK}"
	)
	add_custom_target(Release ALL
		DEPENDS ${RELEASE_DIR}/${MOZILLA_PATH}${PLATFORM_PATH}${CoNetServLibFileName} ${RELEASE_PACK}
	)
	
endif()

# Create debug program
add_executable(Debug ${debug_sources} ${sources} ${platform_sources} ${headers} ${platform_headers})
