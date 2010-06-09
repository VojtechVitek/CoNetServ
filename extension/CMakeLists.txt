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
file(GLOB LIBRARY_PATHS ${CMAKE_SOURCE_DIR}/extension/platform/*)
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

# _firefox:
# 	@echo "  Build Mozilla Firefox add-on:"
# 	@echo "   * build/$(NAME)-$(VERSION).xpi"
# 	@$(RM) build/$(NAME)-$(VERSION).xpi
# 	@cd build/extension && zip -q ../../build/$(NAME)-$(VERSION).xpi \
# 	   chrome.manifest install.rdf `find firefox` \
# 	   `find js` `find images` `find css` index.html `find platform`
#
# _chrome:
# 	@echo "  Build Google Chrome extension:"
# 	@echo "   * build/$(NAME)-$(VERSION).crx"
# 	@$(RM) build/$(NAME)-$(VERSION).crx
# 	@test -f conetserv.pem || { echo "Missing conetserv.pem file."; exit 1; }
# 	@crxmake --pack-extension=build/extension/ --extension-output=build/$(NAME)-$(VERSION).crx \
# 	  --pack-extension-key=conetserv.pem --ignore-file=chrome.manifest --ignore-file=config.xml \
# 	  --ignore-file=about.html --ignore-file=install.rdf --ignore-dir=firefox
#
# _opera:
# 	@echo "  Build Opera widget:"
# 	@echo "   * build/$(NAME)-$(VERSION)-no-plugin.wgt"
# 	@$(RM) build/$(NAME)-$(VERSION)-no-plugin.wgt
# 	@cd build/extension && zip -q ../../build/$(NAME)-$(VERSION)-no-plugin.wgt \
# 	  config.xml \
# 	  `find js` `find images` `find css` index.html