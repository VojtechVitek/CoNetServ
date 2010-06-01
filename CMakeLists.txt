# CoNetServ (Complex Network Services) project
project("CoNetServ")
set(MAJOR_VERSION 1)
set(MINOR_VERSION 2)

# CMake
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakePlugins)

# Build type
# -DCMAKE_BUILD_TYPE=Release
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	# Release build
	message(STATUS "Build type: Release")
	add_definitions(-DNDEBUG)
	set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION})
else()
	# Debug build
	message(STATUS "Build type: Debug")
	add_definitions(-DDEBUG)
 	set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.5)
endif()

# Project strings
set(PROJECT_NAME ${CMAKE_PROJECT_NAME})
set(PROJECT_LONG "Complex Network Services")
set(PROJECT_DESC "Extension which integrates network services into the browser.")

# Plugin strings
set(PLUGIN_NAME ${PROJECT_NAME})
set(PLUGIN_LONG ${PROJECT_LONG})
set(PLUGIN_TYPE "application/x-conetserv")
set(PLUGIN_DESC "<a href=\\\"http://www.${EXTENSION_HOST}/${PROJECT_NAME}/plugin/?version=${VERSION}\\\"> \\
                ${PROJECT_NAME} v${VERSION}</a> - ${PROJECT_LONG} plugin.<br />(c) 2010 FRES-Solutions")
set(PLUGIN_ID   "com.fres-solutions.${PROJECT_NAME}")

# Exension strings
set(EXTENSION_CREATOR "FRES-Solutions")
set(EXTENSION_HOST    "fres-solutions.com")
set(EXTENSION_ID      "${PROJECT_NAME}@${EXTENSION_HOST}")
set(EXTENSION_URL     "http://www.${EXTENSION_HOST}/${PROJECT_NAME}")

# Contributors
if(WIN32)
	set(cat_prog type)
else()
	set(cat_prog cat)
endif()
FILE(TO_NATIVE_PATH ${CMAKE_SOURCE_DIR}/AUTHORS.txt AUTHORS_FILE)
execute_process(
	COMMAND ${cat_prog} ${AUTHORS_FILE}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE AUTHORS
)
string(REGEX MATCHALL "[^\n]+" AUTHORS "${AUTHORS}")

# Plugin
add_subdirectory(plugin)

# Extension
add_subdirectory(extension)

# Documentation
add_subdirectory(doc)