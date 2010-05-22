# CoNetServ documentation

set(DOCUMENTATION_DIR "${CMAKE_SOURCE_DIR}/doc")
set(DOXY_DIR "${DOCUMENTATION_DIR}")
set(DOXY_CONFIG "${DOXY_DIR}/Doxyfile.in")

set(DOXY_SOURCE_DIRS "${CMAKE_SOURCE_DIR}/plugin/ ${CMAKE_SOURCE_DIR}/extension/js/ ${CMAKE_SOURCE_DIR}/")
set(DOXY_EXAMPLE_DIR "")

# Find Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND AND DOXYGEN_EXECUTABLE AND UNIX)

	message(STATUS "Doxygen found.")

	if(EXISTS ${DOXY_CONFIG})
		message(STATUS "Doxygen config file found.")
	else()
		message(FATAL_ERROR "Doxygen config file not found" )
	endif()

	# Copy Doxyfile.in
	configure_file("${DOXY_CONFIG}" "${CMAKE_BINARY_DIR}/Doxyfile")

	# Create doc directory
	add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/doc
	COMMAND rm -rf ${CMAKE_BINARY_DIR}/doc
	COMMAND mkdir ${CMAKE_BINARY_DIR}/doc
	)

	# Run doxygen
	add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/doc/html/index.html
	COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
	DEPENDS "${CMAKE_BINARY_DIR}/Doxyfile"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
	)

	# Target to simply run `make doc'
	add_custom_target(doc DEPENDS ${CMAKE_BINARY_DIR}/doc/html/index.html)

	message(STATUS "Generating API documentation with Doxygen")

else()

	message(STATUS "Not generating API documentation")

endif()