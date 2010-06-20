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

# Chrome and Firefox plugin paths
set(CHROME_LIBRARIES "")
set(FIREFOX_LIBRARIES "")
file(GLOB_RECURSE LIBRARY_PATHS1 ${CMAKE_SOURCE_DIR}/extension/platform/Darwin_*/np${PROJECT_NAME}.dylib)
string(REGEX MATCH "^.*/np${PROJECT_NAME}[.]plugin" LIBRARY_PATHS1 "${LIBRARY_PATHS1}")
file(GLOB_RECURSE LIBRARY_PATHS2 ${CMAKE_SOURCE_DIR}/extension/platform/WINNT_*/np${PROJECT_NAME}.dll)
file(GLOB_RECURSE LIBRARY_PATHS3 ${CMAKE_SOURCE_DIR}/extension/platform/Linux_*/np${PROJECT_NAME}.so)
foreach(LIBRARY_PATH ${LIBRARY_PATHS1} ${LIBRARY_PATHS2} ${LIBRARY_PATHS3})
	# Get rid of double slash, probably bug in CMake file(GLOB_RECURSE ..)
	string(REGEX REPLACE "[/]+" "/" LIBRARY_PATH "${LIBRARY_PATH}")
	file(RELATIVE_PATH LIBRARY_PATH ${CMAKE_SOURCE_DIR}/extension/ ${LIBRARY_PATH})
	if("${CHROME_LIBRARIES}" STREQUAL "")
		set(CHROME_LIBRARIES  "{ \"path\": \"${LIBRARY_PATH}\", \"public\": true }")
	else()
		set(CHROME_LIBRARIES  "${CHROME_LIBRARIES},{ \"path\": \"${LIBRARY_PATH}\", \"public\": true }")
	endif()
	string(REGEX REPLACE "^platform[/]([^/]+).+$" "\\1" LIBRARY_PLATFORM "${LIBRARY_PATH}")
	set(FIREFOX_LIBRARIES "${FIREFOX_LIBRARIES}<em:targetPlatform>${LIBRARY_PLATFORM}</em:targetPlatform>")
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
message(STATUS "Configuring About page")
configure_file("about.html.in" "${CMAKE_SOURCE_DIR}/extension/about.html" IMMEDIATE @ONLY)

# Configure debugging package files:
message(STATUS "Configuring DEBUG browser-specific manifest files")
configure_file("chrome/manifest.json.in" "${CMAKE_SOURCE_DIR}/extension/manifest.json" IMMEDIATE @ONLY)
configure_file("firefox/chrome.manifest.in" "${CMAKE_SOURCE_DIR}/extension/chrome.manifest" IMMEDIATE @ONLY)
configure_file("firefox/install.rdf.in" "${CMAKE_SOURCE_DIR}/extension/install.rdf" IMMEDIATE @ONLY)
configure_file("opera/config.xml.in" "${CMAKE_SOURCE_DIR}/extension/config.xml" IMMEDIATE @ONLY)

# Package generation
if(UNIX)

	# Chrome and Firefox release plugin paths
	set(CHROME_LIBRARIES "")
	set(FIREFOX_LIBRARIES "")
	file(GLOB_RECURSE LIBRARY_PATHS1 ${RELEASE_DIR}/plugins/platform/Darwin_*/np${PROJECT_NAME}.dylib)
	string(REGEX MATCH "^.*/np${PROJECT_NAME}[.]plugin" LIBRARY_PATHS1 "${LIBRARY_PATHS1}")
	file(GLOB_RECURSE LIBRARY_PATHS2 ${RELEASE_DIR}/plugins/platform/WINNT_*/np${PROJECT_NAME}.dll)
	file(GLOB_RECURSE LIBRARY_PATHS3 ${RELEASE_DIR}/plugins/platform/Linux_*/np${PROJECT_NAME}.so)
	foreach(LIBRARY_PATH ${LIBRARY_PATHS1} ${LIBRARY_PATHS2} ${LIBRARY_PATHS3})
		# Get rid of double slash, probably bug in CMake file(GLOB_RECURSE ..)
		string(REGEX REPLACE "[/]+" "/" LIBRARY_PATH "${LIBRARY_PATH}")
		file(RELATIVE_PATH LIBRARY_PATH ${RELEASE_DIR}/plugins/ ${LIBRARY_PATH})
		if("${CHROME_LIBRARIES}" STREQUAL "")
			set(CHROME_LIBRARIES  "{ \"path\": \"${LIBRARY_PATH}\", \"public\": true }")
		else()
			set(CHROME_LIBRARIES  "${CHROME_LIBRARIES},{ \"path\": \"${LIBRARY_PATH}\", \"public\": true }")
		endif()
		string(REGEX REPLACE "^platform[/]([^/]+).+$" "\\1" LIBRARY_PLATFORM "${LIBRARY_PATH}")
		set(FIREFOX_LIBRARIES "${FIREFOX_LIBRARIES}<em:targetPlatform>${LIBRARY_PLATFORM}</em:targetPlatform>")
	endforeach()

	# Configure release package files:
	message(STATUS "Configuring RELEASE browser-specific manifest files")
	configure_file("chrome/manifest.json.in" "${RELEASE_DIR}/chrome/manifest.json" IMMEDIATE @ONLY)
	configure_file("firefox/chrome.manifest.in" "${RELEASE_DIR}/firefox/chrome.manifest" IMMEDIATE @ONLY)
	configure_file("firefox/install.rdf.in" "${RELEASE_DIR}/firefox/install.rdf" IMMEDIATE @ONLY)
	configure_file("opera/config.xml.in" "${RELEASE_DIR}/opera/config.xml" IMMEDIATE @ONLY)


	add_custom_target(ChromeExtension ALL
		DEPENDS ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
	)

	add_custom_target(FirefoxAddOn ALL
		DEPENDS ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi
	)

	add_custom_target(OperaWidget ALL
		DEPENDS ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt
	)

	# Find common files
	file(GLOB_RECURSE common_files1 ${CMAKE_CURRENT_SOURCE_DIR}/js/*.js)
	file(GLOB_RECURSE common_files2 ${CMAKE_CURRENT_SOURCE_DIR}/css/*.css)
	file(GLOB_RECURSE common_files3 ${CMAKE_CURRENT_SOURCE_DIR}/images/*.png)
	file(GLOB_RECURSE common_files4 ${CMAKE_CURRENT_SOURCE_DIR}/images/*.jpg)
	file(GLOB_RECURSE common_files5 ${CMAKE_CURRENT_SOURCE_DIR}/images/*.gif)
	set(common_files
		${common_files1}
		${common_files2}
		${common_files3}
		${common_files4}
		${common_files5}
		${CMAKE_CURRENT_SOURCE_DIR}/index.html
		${CMAKE_CURRENT_SOURCE_DIR}/about.html
	)

	# Find platform files
	file(GLOB_RECURSE platform_files ${RELEASE_DIR}/plugins/*)

	# Chrome files
	set(chrome_files
		${common_files}
		${platform_files}
	)
	foreach(chrome_file ${chrome_files})
		file(RELATIVE_PATH chrome_file_relative ${CMAKE_CURRENT_SOURCE_DIR}/ ${chrome_file})
		set(chrome_files_relative ${chrome_files_relative} "${RELEASE_DIR}/chrome/${chrome_file_relative}")
		add_custom_command(
			OUTPUT ${RELEASE_DIR}/chrome/${chrome_file_relative}
			DEPENDS ${chrome_file}
			COMMAND cmake -E copy ${chrome_file} ${RELEASE_DIR}/chrome/${chrome_file_relative}
		)
	endforeach()

	# Firefox files
	file(GLOB_RECURSE firefox_files1 ${CMAKE_CURRENT_SOURCE_DIR}/firefox/locale/*)
	set(firefox_files
		${common_files}
		${platform_files}
		${firefox_files1}
		${CMAKE_CURRENT_SOURCE_DIR}/firefox/overlay.css
		${CMAKE_CURRENT_SOURCE_DIR}/firefox/overlay.xul
	)
	foreach(firefox_file ${firefox_files})
		file(RELATIVE_PATH firefox_file_relative ${CMAKE_CURRENT_SOURCE_DIR}/ ${firefox_file})
		set(firefox_files_relative ${firefox_files_relative} "${RELEASE_DIR}/firefox/${firefox_file_relative}")
		add_custom_command(
			OUTPUT ${RELEASE_DIR}/firefox/${firefox_file_relative}
			DEPENDS ${firefox_file}
			COMMAND cmake -E copy ${firefox_file} ${RELEASE_DIR}/firefox/${firefox_file_relative}
		)
	endforeach()

	# Opera files
	set(opera_files
		${common_files}
	)
	foreach(opera_file ${opera_files})
		file(RELATIVE_PATH opera_file_relative ${CMAKE_CURRENT_SOURCE_DIR}/ ${opera_file})
		set(opera_files_relative ${opera_files_relative} "${RELEASE_DIR}/opera/${opera_file_relative}")
		add_custom_command(
			OUTPUT ${RELEASE_DIR}/opera/${opera_file_relative}
			DEPENDS ${opera_file}
			COMMAND cmake -E copy ${opera_file} ${RELEASE_DIR}/opera/${opera_file_relative}
		)
	endforeach()

	# Chrome package build
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
		DEPENDS ${chrome_files_relative}
		WORKING_DIRECTORY ${RELEASE_DIR}/chrome
		COMMAND rm -rf ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
		COMMAND zip -q -r ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.zip *
		COMMAND crxmake --extension-output=${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx
		                --pack-extension=${RELEASE_DIR}/chrome
		                --pack-extension-key=${CMAKE_SOURCE_DIR}/conetserv.pem
		COMMENT "Generating Chrome Exension into ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.crx (.zip)"
	)

	# Firefox package build
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi
		DEPENDS ${firefox_files_relative}
		WORKING_DIRECTORY ${RELEASE_DIR}/firefox
		COMMAND rm -rf ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi
		COMMAND zip -q -r ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi *
		COMMENT "Generating Firefox Add-on into ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.xpi"
	)

	# Opera package build
	add_custom_command(
		OUTPUT ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt
		DEPENDS ${opera_files_relative}
		WORKING_DIRECTORY ${RELEASE_DIR}/opera
		COMMAND rm -rf ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt
		COMMAND zip -q -r ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt *
		COMMENT "Generating Opera Widget into ${RELEASE_DIR}/${PROJECT_NAME}-${VERSION}.wgt"
	)

else()

	message(STATUS "Not generating browser packages. Only UNIX is supported.")

endif()