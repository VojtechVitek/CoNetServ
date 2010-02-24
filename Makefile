include config.mk

NAME=conetserv

.PHONY: all build plugin-x86_64 firefox-common firefox-linux-x86_64

all: firefox-linux-x86_64

build:
	@mkdir -p ../build

plugin-x86_64:
	@cd plugin && make

firefox-common:
	@rm -rf firefox/platform/

firefox-linux-x86_64: plugin-x86_64 firefox-common
	@echo "Building Linux x86_64 XPI..."
	@$(RM) build/conetserv-linux-x86_64.xpi
	@mkdir -p firefox/platform/Linux_x86_64-gcc3/plugins
	@cp -a plugin/npconetserv.so firefox/platform/Linux_x86_64-gcc3/plugins/
	@zip -q build/conetserv-linux-x86_64.xpi \
	        firefox/chrome.manifest firefox/install.rdf \
		`find firefox/platform` `find firefox/content` \
		`find firefox/locale` `find firefox/skin`

