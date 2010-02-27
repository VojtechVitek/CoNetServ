include config.mk

NAME=conetserv

.PHONY: all build plugin-x86_64 firefox-common firefox-linux-x86_64

all: firefox chrome opera

build:
	@mkdir -p ../build

plugin-x86_64:
	@cd plugin && make

firefox: 
	@echo "Building Firefox add-on..."
	@$(RM) build/firefox/conetserv.xpi
	@mkdir -p build/firefox/
	@cd extension && zip -q ../build/firefox/conetserv.xpi \
	        chrome.manifest install.rdf \
		`find icon*.png` `find firefox` \
		`find platform` `find jquery` \
		index.html index.js

chrome:
	@echo "Building Chrome extension..."
	@$(RM) build/chrome/conetserv.crx
	@mkdir -p build/chrome/
	@crxmake --pack-extension=extension/ --extension-output=build/chrome/conetserv.crx --pack-extension-key=conetserv.pem

opera:
	@echo "Building Opera widget..."
	@$(RM) build/opera/conetserv-no-plugin.wgt
	@mkdir -p build/opera/
	@cd extension && zip -q ../build/opera/conetserv-no-plugin.wgt \
	        config.xml `find icon*.png` \
		index.html \
		`find jquery/` \
		index.html index.js


