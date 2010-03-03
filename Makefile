include config.mk

NAME=CoNetServ
LIBNAME=np$(NAME)
ARCHS=Linux_x86-gcc3.so Linux_x86_64-gcc3.so WINNT_x86-MSVC.dll Darwin_x86-gcc3.dylib

.PHONY: all build common firefox chrome opera

all: common firefox chrome opera

common:
	@echo "==================================================================="
	@echo "              CoNetServ - package generator                        "
	@echo "-------------------------------------------------------------------"
	@echo " * Generates browser-specific extension packages.                  "
	@echo " * Need pre-built libraries for all supported archs and platforms. "
	@echo "==================================================================="
	@mkdir -p build/
	@for i in $(ARCHS); do \
	  test -f build/$(LIBNAME)_$$i || \
	  { echo "Missing build/$(LIBNAME)_$$i"; exit 1; }; \
	  mkdir -p extension/platform/$$(echo $$i | cut -d'.' -f1)/plugins/; \
	  cp -t extension/platform/$$(echo $$i | cut -d'.' -f1)/plugins/ build/$(LIBNAME)_$$i; \
	done

firefox:
	@echo "Building Mozilla Firefox add-on... build/$(NAME)-$(VERSION).xpi"
	@$(RM) build/$(NAME)-$(VERSION).xpi
	@cd extension && zip -q ../build/$(NAME)-$(VERSION).xpi \
	   chrome.manifest install.rdf `find firefox` \
	   `find js` `find images` `find css` index.html `find jquery` \
	   `find platform` `find jquery`

chrome:
	@echo "Building Google Chrome extension... build/$(NAME)-$(VERSION).crx"
	@$(RM) build/$(NAME)-$(VERSION).crx
	@test -f conetserv.pem || { echo "Missing conetserv.pem file."; exit 1; }
	@crxmake --pack-extension=extension/ --extension-output=build/$(NAME)-$(VERSION).crx \
	  --pack-extension-key=conetserv.pem --ignore-file=chrome.manifest --ignore-file=config.xml \
	  --ignore-file=install.rdf --ignore-dir=firefox

opera:
	@echo "Building Opera widget... build/$(NAME)-$(VERSION)-no-plugin.wgt"
	@$(RM) build/$(NAME)-$(VERSION)-no-plugin.wgt
	@cd extension && zip -q ../build/$(NAME)-$(VERSION)-no-plugin.wgt \
	  config.xml \
	  `find js` `find images` `find css` index.html `find jquery`

