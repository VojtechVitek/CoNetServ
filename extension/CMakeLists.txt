# CoNetServ extension

# Build date
if(UNIX)
	exec_program(date
		ARGS +%Y-%m-%d
		OUTPUT_VARIABLE BUILD_DATE
	)
else()
	set(BUILD_DATE "1970-01-01")
endif()

# Chrome plugin paths
file(GLOB_RECURSE LIBRARY_PATHS ${CMAKE_SOURCE_DIR}/extension/platform/*np${PROJECT_NAME}.*)
foreach(LIBRARY_PATH ${LIBRARY_PATHS})
	file(RELATIVE_PATH LIBRARY_PATH ${CMAKE_SOURCE_DIR}/extension/ ${LIBRARY_PATH})
	if("${CHROME_LIBRARIES}" STREQUAL "")
		set(CHROME_LIBRARIES  "{ \"path\": \"${LIBRARY_PATH}\", \"public\": true }")
	else()
		set(CHROME_LIBRARIES  "${CHROME_LIBRARIES},{ \"path\": \"${LIBRARY_PATH}\", \"public\": true }")
	endif()
endforeach()

# Firefox plugin paths
file(GLOB_RECURSE LIBRARY_PATHS ${CMAKE_SOURCE_DIR}/extension/platform/*)
foreach(LIBRARY_PATH ${LIBRARY_PATHS})
	file(RELATIVE_PATH LIBRARY_PATH ${CMAKE_SOURCE_DIR}/extension/platform ${LIBRARY_PATH})
	if("${FIREFOX_LIBRARIES}" STREQUAL "")
		set(FIREFOX_LIBRARIES "<em:targetPlatform>${LIBRARY_PATH}</em:targetPlatform>")
	else()
		set(FIREFOX_LIBRARIES "${FIREFOX_LIBRARIES}<em:targetPlatform>${LIBRARY_PATH}</em:targetPlatform>")
	endif()
endforeach()

# Firefox contributors
foreach(AUTHOR ${AUTHORS})
	string(REGEX REPLACE "[<]" "&lt;" AUTHOR "${AUTHOR}")
	string(REGEX REPLACE "[>]" "&gt;" AUTHOR "${AUTHOR}")
	if("${FIREFOX_AUTHORS}" STREQUAL "")
		set(FIREFOX_AUTHORS "<em:contributor>${AUTHOR}</em:contributor>")
	else()
		set(FIREFOX_AUTHORS "${FIREFOX_AUTHORS}<em:contributor>${AUTHOR}</em:contributor>")
	endif()
endforeach()

# About page contributors
foreach(AUTHOR ${AUTHORS})
	string(REGEX REPLACE "[<]" "&lt;" AUTHOR "${AUTHOR}")
	string(REGEX REPLACE "[>]" "&gt;" AUTHOR "${AUTHOR}")
	string(REGEX REPLACE "[@]" "@<span class="hidden">&#185;</span>" AUTHOR "${AUTHOR}")
	if("${ABOUT_AUTHORS}" STREQUAL "")
		set(ABOUT_AUTHORS "${AUTHOR}")
	else()
		set(ABOUT_AUTHORS "${ABOUT_AUTHORS}<br />${AUTHOR}")
	endif()
endforeach()

# Configure about page
message(STATUS "Configuring about page")
configure_file("about.html.in" "${CMAKE_SOURCE_DIR}/extension/about.html" IMMEDIATE @ONLY)

# Configure debugging package files:
message(STATUS "Configuring browser-specific manifest files")
configure_file("chrome/manifest.json.in" "${CMAKE_SOURCE_DIR}/extension/manifest.json" IMMEDIATE @ONLY)
configure_file("firefox/chrome.manifest.in" "${CMAKE_SOURCE_DIR}/extension/chrome.manifest" IMMEDIATE @ONLY)
configure_file("firefox/install.rdf.in" "${CMAKE_SOURCE_DIR}/extension/install.rdf" IMMEDIATE @ONLY)
configure_file("opera/config.xml.in" "${CMAKE_SOURCE_DIR}/extension/config.xml" IMMEDIATE @ONLY)

# Package generation
if(UNIX)

	get_filename_component(RELEASE_DIR ${CMAKE_BINARY_DIR}/../ ABSOLUTE)
	file(MAKE_DIRECTORY ${RELEASE_DIR}/platform)
	file(MAKE_DIRECTORY ${RELEASE_DIR}/chrome)
	file(REMOVE_RECURSE ${RELEASE_DIR}/chrome/*)
	file(MAKE_DIRECTORY ${RELEASE_DIR}/firefox)
	file(REMOVE_RECURSE ${RELEASE_DIR}/firefox/*)
	file(MAKE_DIRECTORY ${RELEASE_DIR}/opera)
	file(REMOVE_RECURSE ${RELEASE_DIR}/opera/*)

	# Find common files
	file(GLOB_RECURSE common_files1 ${CMAKE_CURRENT_SOURCE_DIR}/js/*)
	file(GLOB_RECURSE common_files2 ${CMAKE_CURRENT_SOURCE_DIR}/css/*)
	file(GLOB_RECURSE common_files3 ${CMAKE_CURRENT_SOURCE_DIR}/images/*)
	set(common_files
		${common_files1}
		${common_files2}
		${common_files3}
		${CMAKE_CURRENT_SOURCE_DIR}/index.html
		${CMAKE_CURRENT_SOURCE_DIR}/about.html
	)

	# Find platform files
	file(GLOB_RECURSE platform_files ${CMAKE_CURRENT_SOURCE_DIR}/platform/*)

	# Chrome files
	set(chrome_files
		${CMAKE_CURRENT_SOURCE_DIR}/manifest.json
		${common_files}
		${platform_files}
	)
	foreach(chrome_file ${chrome_files})
		set(chrome_file_string "${chrome_file_string} ${chrome_file}")
	endforeach()
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/chrome
		COMMAND cp -r -t ${RELEASE_DIR}/chrome ${chrome_file_string}
	)

	# Firefox files
	file(GLOB_RECURSE firefox_files1 ${CMAKE_CURRENT_SOURCE_DIR}/firefox/locale/*)
	set(firefox_files
		${CMAKE_CURRENT_SOURCE_DIR}/install.rdf
		${CMAKE_CURRENT_SOURCE_DIR}/chrome.manifest
		${common_files}
		${platform_files}
		${firefox_files1}
		${CMAKE_CURRENT_SOURCE_DIR}/firefox/overlay.css
		${CMAKE_CURRENT_SOURCE_DIR}/firefox/overlay.xul
	)
	foreach(firefox_file ${firefox_files})
		set(firefox_file_string "${firefox_file_string} ${firefox_file}")
	endforeach()
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/firefox
		COMMAND cp -r -t ${RELEASE_DIR}/firefox ${firefox_file_string}
	)

	# Opera files
	set(opera_files
		${CMAKE_CURRENT_SOURCE_DIR}/config.xml
		${common_files}
	)
	foreach(opera_file ${opera_files})
		set(opera_file_string "${opera_file_string} ${opera_file}")
	endforeach()
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/opera
		COMMAND cp -r -t ${RELEASE_DIR}/opera ${opera_file_string}
	)

	# Chrome package build
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/extension/chrome
		COMMAND rm -rf ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
		COMMAND crxmake --extension-output=${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
				--pack-extension=${CMAKE_SOURCE_DIR}/extension/chrome --pack-extension-key=${CMAKE_SOURCE_DIR}/conetserv.pem
	)

	add_custom_target(ChromeExtension ALL
		DEPENDS ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
	)

	# Firefox package build
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/extension/firefox
		COMMAND rm -rf ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi
		COMMAND zip -q ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi *
	)

	add_custom_target(FirefoxAddOn ALL
		DEPENDS ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi
	)

	# Opera package build
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/extension/opera
		COMMAND rm -rf ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt
		COMMAND zip -q ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt *
	)

	add_custom_target(OperaWidget ALL
		DEPENDS ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt
	)

endif()