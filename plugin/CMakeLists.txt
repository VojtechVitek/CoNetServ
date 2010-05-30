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
if(APPLE)
	include(apple/CMakeLists.txt)
	include(unix/CMakeLists.txt)
elseif(UNIX)
	include(unix/CMakeLists.txt)
elseif(WIN32)
	include(win/CMakeLists.txt)
else()
	message(FATAL ERROR "Sorry, your operating system is not supported.")
endif()

# Create shared library
add_library(CoNetServ SHARED ${sources} ${platform_sources} ${headers} ${platform_headers})
set_target_properties(CoNetServ PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(CoNetServ PROPERTIES PREFIX "np")

# Export release shared library (force simple packaging)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME})
	get_target_property(CoNetServLibFile CoNetServ LOCATION)
	file(RELATIVE_PATH CoNetServLibFileName ${CMAKE_CURRENT_BINARY_DIR} ${CoNetServLibFile})
	add_custom_command(
		OUTPUT  ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
		COMMAND ${CMAKE_COMMAND} -E copy
		${CoNetServLibFile} ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
		DEPENDS ${CoNetServLibFile}
		COMMENT "Exporting shared library: ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}"
	)
	file(RELATIVE_PATH RELEASE_PATH ${CMAKE_BINARY_DIR}/../ ${CMAKE_BINARY_DIR}/../${PROJECT_NAME}-${VERSION}-${CMAKE_SYSTEM_NAME}.tgz)
	add_custom_command(
		OUTPUT  ${CMAKE_BINARY_DIR}/../${PROJECT_NAME}-${VERSION}-${CMAKE_SYSTEM_NAME}.tgz
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME}
		COMMAND ${CMAKE_COMMAND} -E tar -czf
		${CMAKE_BINARY_DIR}/../${PROJECT_NAME}-${VERSION}-${CMAKE_SYSTEM_NAME}.tgz *
		DEPENDS ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
		COMMENT "Packing release plugin into: ../${RELEASE_PATH}"
	)
	add_custom_target(Release ALL
		DEPENDS ${CMAKE_BINARY_DIR}/../${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
		${CMAKE_BINARY_DIR}/../${PROJECT_NAME}-${VERSION}-${CMAKE_SYSTEM_NAME}.tgz
	)
	
endif()

# Create debug program
add_executable(Debug ${debug_sources} ${sources} ${platform_sources} ${headers} ${platform_headers})
