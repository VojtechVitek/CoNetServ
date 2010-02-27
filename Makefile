include config.mk

NAME=conetserv

.PHONY: all build plugin-x86_64 firefox-common firefox-linux-x86_64

all: firefox-linux-x86_64 chrome-linux-x86_64 opera-linux-x86_64

build:
	@mkdir -p ../build

plugin-x86_64:
	@cd plugin && make

firefox-linux-x86_64: plugin-x86_64
	@echo "Building Firefox add-on x86_64..."
	@$(RM) build/firefox/conetserv-linux-x86_64.xpi
	@mkdir -p build/firefox/
	@mkdir -p extension/platform/Linux_x86_64-gcc3/plugins
	@cp -a plugin/npconetserv.so extension/platform/Linux_x86_64-gcc3/plugins/
	@cd extension && zip -q ../build/firefox/conetserv-linux-x86_64.xpi \
	        chrome.manifest install.rdf \
		`find icon*.png` `find firefox` \
		`find platform` `find jquery` \
		popup.html popup.js

chrome-linux-x86_64: plugin-x86_64
	@echo "Building Chrome extension x86_64..."
	@$(RM) build/chrome/conetserv-linux-x86_64.crx
	@mkdir -p build/chrome/
	@mkdir -p extension/platform/Linux_x86_64-gcc3/plugins
	@cp -a plugin/npconetserv.so extension/platform/Linux_x86_64-gcc3/plugins/
	@crxmake --pack-extension=extension/ --extension-output=build/chrome/conetserv-linux-x86_64.crx --pack-extension-key=conetserv.pem
#	@cd extension && zip -q ../build/chrome/conetserv-linux-x86_64.xpi \
	        chrome.manifest install.rdf \
		`find icon*.png` `find firefox/` \
		`find platform/` `find jquery/` \
		popup.html popup.js

opera-linux-x86_64: plugin-x86_64
	@echo "Building Opera widget x86_64..."
	@$(RM) build/opera/conetserv-no-plugin.wgt
	@mkdir -p build/opera/
	@cd extension && zip -q ../build/opera/conetserv-no-plugin.wgt \
	        config.xml `find icon*.png` \
		index.html \
		`find jquery/` \
		popup.html popup.js


