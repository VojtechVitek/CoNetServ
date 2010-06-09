# CoNetServ (Complex Network Services) project
project("CoNetServ")
set(MAJOR_VERSION 1)
set(MINOR_VERSION 2)
set(PATCH_VERSION 0)

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
set(PROJECT_HOST "${PROJECT_HOSTNAME}${PROJECT_HOST_TLD}")
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