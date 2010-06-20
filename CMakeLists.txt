# CoNetServ (Complex Network Services) project
project("CoNetServ")
set(MAJOR_VERSION 1)
set(MINOR_VERSION 3)
set(PATCH_VERSION 0)

# CMake
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakePlugins)

# Release extension packages and/or plugins are built in first level build directory
# This means, that developer should build project in second level build directory,
# eg. build/Linux or build/Windows.
# The final packages (which contains of more platform builds) are made in the first
# level build directory, eg. build/.
get_filename_component(RELEASE_DIR ${CMAKE_BINARY_DIR}/../ ABSOLUTE)
get_filename_component(CMAKE_SOURCE_DIR_TEST ${CMAKE_BINARY_DIR}/../../ ABSOLUTE)
if(NOT "${CMAKE_SOURCE_DIR_TEST}" STREQUAL "${CMAKE_SOURCE_DIR}")
	message(FATAL_ERROR "Please, build project in sub-subdirectory of your source code, eg. build/${CMAKE_SYSTEM_NAME}.")
endif()

# Debug extensions are built in directory extension (for simple development process)
get_filename_component(DEBUG_DIR ${CMAKE_SOURCE_DIR}/extension/ ABSOLUTE)

# Build type
# -DCMAKE_BUILD_TYPE=Release
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	# Release build
	message(STATUS "Build type: Release")
	add_definitions(-DNDEBUG)
	set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
else()
	# Debug build
	message(STATUS "Build type: Debug")
	add_definitions(-DDEBUG)
	set(PATCH_VERSION 99)
endif()

# Project strings
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(PROJECT_NAME ${CMAKE_PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} PROJECT_LOW)
set(PROJECT_LONG "Complex Network Services")
set(PROJECT_COMPANY "FRES-Solutions")
set(PROJECT_COPY "(c) 2010 ${PROJECT_COMPANY}")
set(PROJECT_HOSTNAME "fres-solutions")
set(PROJECT_HOST_TLD "com")
set(PROJECT_HOST "${PROJECT_HOSTNAME}.${PROJECT_HOST_TLD}")
set(PROJECT_URL "http://www.${PROJECT_HOSTNAME}.${PROJECT_HOST_TLD}/${PROJECT_NAME}")

# Exension strings
set(EXTENSION_SLOGAN "Extension which integrates network services into the browser.")
set(EXTENSION_DESC "${PROJECT_LONG} v${VERSION}. ${EXTENSION_SLOGAN} ${PROJECT_COPY}")
set(EXTENSION_ID "${PROJECT_LOW}@${PROJECT_HOSTNAME}.${PROJECT_HOST_TLD}")

# Plugin strings
set(PLUGIN_SLOGAN "Plugin which integrates network services into the browser.")
set(PLUGIN_NAME ${PROJECT_NAME})
set(PLUGIN_LONG ${PROJECT_LONG})
set(PLUGIN_TYPE "application/x-${PROJECT_LOW}")
set(PLUGIN_DESC "${PROJECT_LONG} v${VERSION}. ${PLUGIN_SLOGAN} ${PROJECT_COPY}")
set(PLUGIN_ID   "${PROJECT_HOST_TLD}.${PROJECT_HOSTNAME}.${PROJECT_LOW}")

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
