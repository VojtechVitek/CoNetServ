# Project
project(CoNetServ)
set(MAJOR_VERSION 1)
set(MINOR_VERSION 3)
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION})

# CMake
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakePlugins")
set(DOCUMENTATION_PATH "${CMAKE_SOURCE_DIR}/doc")

# Set library prefixes
SET(LIBDIR "lib${LIB_SUFFIX}")

if(UNIX)
	add_definitions(-std=gnu99)
endif()

# Build type
message("-- Build type: ${CMAKE_BUILD_TYPE}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	# Release build
	add_definitions(-DVERSION="${VERSION}")
	add_definitions(-DNDEBUG)
	add_definitions(-O2)
else()
	# Debug build
	add_definitions(-DVERSION="${VERSION}.1")
	add_definitions(-DDEBUG)
	add_definitions(-g)
endif()

# Subdirectories
add_subdirectory(plugin)
add_subdirectory(extension)
add_subdirectory(doc)

# Custom targets
add_custom_target(packages "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/Packages.cmake")
add_custom_target(doxygen "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/Doxygen.cmake")