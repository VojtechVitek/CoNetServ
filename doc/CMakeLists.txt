# CoNetServ documentation

set(DOCUMENTATION_DIR "${CMAKE_SOURCE_DIR}/doc")

set(DOXY_CONFIG "${DOCUMENTATION_DIR}/Doxyfile.in")
set(DOXY_EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/extension/")
set(DOXY_IMAGE_FILES "${CMAKE_SOURCE_DIR}/plugin/api/")

file(GLOB files1 ${CMAKE_SOURCE_DIR}/plugin/*.[ch])
file(GLOB files2 ${CMAKE_SOURCE_DIR}/plugin/api/*.h)
file(GLOB files3 ${CMAKE_SOURCE_DIR}/plugin/unix/*.[ch])
file(GLOB files4 ${CMAKE_SOURCE_DIR}/plugin/win/*.[ch])
file(GLOB files5 ${CMAKE_SOURCE_DIR}/extension/js/*.js)
set(files_doc ${files1} ${files2} ${files3} ${files4} ${files5} ${CMAKE_BINARY_DIR}/doc/about.h)
foreach(file ${files_doc})
	set(DOXY_SOURCE_FILES "${DOXY_SOURCE_FILES} ${file}")
endforeach()

# Find Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND AND DOXYGEN_EXECUTABLE AND UNIX)

	if(NOT EXISTS ${DOXY_CONFIG})
		message(FATAL_ERROR "Doxygen config file not found" )
	endif()

	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc)

	# Copy and configure Doxyfile.in
	configure_file("${DOXY_CONFIG}" "${CMAKE_BINARY_DIR}/doc/Doxyfile")

	# Copy and configure about.h.in
	configure_file("${DOCUMENTATION_DIR}/about.h.in" "${CMAKE_BINARY_DIR}/doc/about.h")

	# Run doxygen
	add_custom_command(
		OUTPUT "${CMAKE_BINARY_DIR}/doc/html/index.html"
		COMMAND rm -rf ${CMAKE_BINARY_DIR}/doc/html/
		COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/doc/Doxyfile"
		DEPENDS "${CMAKE_BINARY_DIR}/doc/Doxyfile"
		DEPENDS "${CMAKE_BINARY_DIR}/doc/about.h"
		DEPENDS ${files_doc}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
	)

	# Target to simply run `make doc'
	add_custom_target(doc
		DEPENDS "${CMAKE_BINARY_DIR}/doc/html/index.html"
		COMMENT "Generating Doxygen documentation."
	)

	if(DOXYGEN_DOT_FOUND)
		message(STATUS "Configuring Doxygen and Graphviz Dot tool")
	else()
		message(STATUS "Configuring Doxygen")
	endif()

else()

	message(STATUS "Can't configure Doxygen")

endif()
