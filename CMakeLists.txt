# CoNetServ (Complex Network Services) project
project("CoNetServ")
set(MAJOR_VERSION 1)
set(MINOR_VERSION 2)

# Project strings
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION})
set(PROJECT_NAME ${CMAKE_PROJECT_NAME})
set(PROJECT_LONG "Complex Network Services")
set(PROJECT_DESC "Extension which integrates network services into the browser.")

# Plugin strings
set(PLUGIN_NAME ${PROJECT_NAME})
set(PLUGIN_LONG ${PROJECT_LONG})
set(PLUGIN_TYPE "application/x-conetserv")
set(PLUGIN_DESC "<a href=\\\"http://www.fres-solutions.com/conetserv/plugin/?version=${VERSION}\\\"> \\
                ${PROJECT_NAME} v${VERSION}</a> - ${PROJECT_LONG} plugin.<br />(c) 2010 FRES-Solutions")
set(PLUGIN_BUNDLE "com.fres-solutions.@PROJECT_NAME@")

# CMake
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakePlugins")

# Build type
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	# Release build
	message(STATUS "Build type: Release")
	add_definitions(-DNDEBUG)
else()
	# Debug build
	message(STATUS "Build type: Debug")
	set(VERSION ${VERSION}.5)
	add_definitions(-DDEBUG)
endif()

# Plugin
add_subdirectory(plugin)

# Extension
add_subdirectory(extension)

# Documentation
add_subdirectory(doc)