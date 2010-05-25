# CoNetServ documentation

set(DOCUMENTATION_DIR "${CMAKE_SOURCE_DIR}/doc")

set(DOXY_CONFIG "${DOCUMENTATION_DIR}/Doxyfile.in")
set(DOXY_EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/plugin/api/")
set(DOXY_IMAGE_FILES "${CMAKE_SOURCE_DIR}/plugin/api/")

file(GLOB files0 ${CMAKE_BINARY_DIR}/*.h)
file(GLOB files1 ${CMAKE_SOURCE_DIR}/plugin/*.[ch])
file(GLOB files2 ${CMAKE_SOURCE_DIR}/plugin/api/*.h)
file(GLOB files3 ${CMAKE_SOURCE_DIR}/plugin/unix/*.[ch])
file(GLOB files4 ${CMAKE_SOURCE_DIR}/plugin/win/*.[ch])
file(GLOB files5 ${CMAKE_SOURCE_DIR}/extension/js/*.js)
foreach(file ${files0} ${files1} ${files2} ${files3} ${files4} ${files5})
	set(DOXY_SOURCE_FILES "${DOXY_SOURCE_FILES} ${file}")
endforeach()

# Find Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND AND DOXYGEN_EXECUTABLE AND UNIX)

	if(NOT EXISTS ${DOXY_CONFIG})
		message(FATAL_ERROR "Doxygen config file not found" )
	endif()

	# Copy Doxyfile.in
	configure_file("${DOXY_CONFIG}" "${CMAKE_BINARY_DIR}/doc/Doxyfile")

	# Create doc directory
	add_custom_command(
		OUTPUT ${CMAKE_BINARY_DIR}/doc
		COMMAND rm -rf ${CMAKE_BINARY_DIR}/doc
		COMMAND mkdir ${CMAKE_BINARY_DIR}/doc
	)

	# Run doxygen
	add_custom_command(
		OUTPUT ${CMAKE_BINARY_DIR}/doc/html/index.html
		COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/doc/Doxyfile"
		DEPENDS "${CMAKE_BINARY_DIR}/Doxyfile"
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
	)

	# Target to simply run `make doc'
	add_custom_target(doc DEPENDS ${CMAKE_BINARY_DIR}/doc/html/index.html)

	if(DOXYGEN_DOT_FOUND)
		message(STATUS "Generating API documentation with Doxygen and Graphviz Dot tool")
	else()
		message(STATUS "Generating API documentation with Doxygen")
	endif()

else()

	message(STATUS "Not generating API documentation")

endif()