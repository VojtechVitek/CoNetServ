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
	message(STATUS ${CMAKE_SYSTEM_NAME})
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../export/${CMAKE_SYSTEM_NAME})
	get_target_property(CoNetServLibFile CoNetServ LOCATION)
	file(RELATIVE_PATH CoNetServLibFileName ${CMAKE_CURRENT_BINARY_DIR} ${CoNetServLibFile})
	add_custom_command(
		OUTPUT  ${CMAKE_BINARY_DIR}/../export/${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
		COMMAND ${CMAKE_COMMAND} -E copy
		${CoNetServLibFile} ${CMAKE_BINARY_DIR}/../export/${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
		DEPENDS ${CoNetServLibFile}
		COMMENT "Exporting shared library: ${CMAKE_BINARY_DIR}/../export/${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}"
	)
	add_custom_target(export ALL
	DEPENDS ${CMAKE_BINARY_DIR}/../export/${CMAKE_SYSTEM_NAME}/${CoNetServLibFileName}
	)
endif()

# Create debug program
add_executable(debug ${debug_sources} ${sources} ${platform_sources} ${headers} ${platform_headers})
