# CoNetServ (Complex Network Services) project
project(CoNetServ)
set(MAJOR_VERSION 1)
set(MINOR_VERSION 2)

set(PROJECT ${CMAKE_PROJECT_NAME})
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION})

# CMake
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakePlugins")

# Build type
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	# Release build
	message(STATUS "Build type: Release")
	add_definitions(-DVERSION="${VERSION}")
	add_definitions(-DNDEBUG)
else()
	# Debug build
	message(STATUS "Build type: Debug")
	add_definitions(-DVERSION="${VERSION}.1")
	add_definitions(-DDEBUG)
endif()

# Plugin
add_subdirectory(plugin)

# Extension
add_subdirectory(extension)

# Documentation
add_subdirectory(doc)