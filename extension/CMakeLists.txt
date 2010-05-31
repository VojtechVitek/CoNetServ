# CoNetServ extension

# Build date
if(UNIX)
	exec_program(date
		ARGS +%Y-%m-%d
		OUTPUT_VARIABLE BUILD_DATE
	)
else()
	set(BUILD_DATE "2010-02-01")
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

# Configure debugging package files:
configure_file("chrome/manifest.json.in" "${CMAKE_SOURCE_DIR}/extension/manifest.json")
configure_file("firefox/chrome.manifest.in" "${CMAKE_SOURCE_DIR}/extension/chrome.manifest")
configure_file("firefox/install.rdf.in" "${CMAKE_SOURCE_DIR}/extension/install.rdf")
configure_file("opera/config.xml.in" "${CMAKE_SOURCE_DIR}/extension/config.xml")

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