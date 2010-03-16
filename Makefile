NAME=CoNetServ
LIBNAME=np$(NAME)
ARCHS=Linux_x86-gcc3 Linux_x86_64-gcc3 WINNT_x86-MSVC Darwin_x86-gcc3

include VERSION

.PHONY: all pre doc
all: pre _firefox _chrome _opera doc

pre:
	@echo "CoNetServ - package generator"
	@echo "============================="
	@echo "Package strings:"
	@echo "  Version.. \"$(VERSION)\""
	@echo "  Build-date.. \"$(BUILDDATE)\""
	@echo "-----------------------------"
	@echo "Package files:"
	@echo "  Copy current extension files.."
	@mkdir -p build/
	@rm -rf build/extension
	@cp -r extension build/
	@rm -rf build/extension/platform
	@echo "  Check pre-built libraries.."
	@for i in $(ARCHS); do \
	  find build/$$i | while read l; do echo "  * $$l"; done; \
	  test -d build/$$i || exit 1; \
	  cp -L -r build/$$i build/extension/platform/ ; \
	done
	@echo "  Substitute \"@VERSION@\" by \"$(VERSION)\" in extension files.."
	@sed -e '/@ABOUT.HTML@/r build/extension/about.html' \
	  -e 's/@ABOUT.HTML@//' build/extension/index.html > build/extension/index.html.tmp; \
	  mv build/extension/index.html.tmp build/extension/index.html
	@find build/extension/ build/*/*.plist -type f -print | while read i; do \
	  sed -e 's/@VERSION@/$(VERSION)/g' -e 's/@BUILDDATE@/$(BUILDDATE)/g' \
	  $$i > $$i.tmp && mv $$i.tmp $$i; \
	done
	@echo "-----------------------------"
	@echo "Generate extension packages:"

doc:
	@echo "-----------------------------"
	@echo "Generate doxygen documentation:"
	@echo "  Substite \"@VERSION@\" by \"$(VERSION)\" in doxygen files.."
	@cp doc/Doxyfile build/Doxyfile
	@echo build/Doxyfile | while read i; do \
	  sed -e 's/@VERSION@/$(VERSION)/g' -e 's/@BUILDDATE@/$(BUILDDATE)/g' \
	  $$i > $$i.tmp && mv $$i.tmp $$i; \
	done
	@echo "  Generate doxygen documentation.."
	@cd build && doxygen 2>/dev/null 1>&2


.PHONY:_firefox _chrome _opera
_firefox:
	@echo "  Build Mozilla Firefox add-on.."
	@echo "   * build/$(NAME)-$(VERSION).xpi"
	@$(RM) build/$(NAME)-$(VERSION).xpi
	@cd build/extension && zip -q ../../build/$(NAME)-$(VERSION).xpi \
	   chrome.manifest install.rdf `find firefox` \
	   `find js` `find images` `find css` index.html `find jquery` \
	   `find platform` `find jquery`

_chrome:
	@echo "  Build Google Chrome extension.."
	@echo "   * build/$(NAME)-$(VERSION).crx"
	@$(RM) build/$(NAME)-$(VERSION).crx
	@test -f conetserv.pem || { echo "Missing conetserv.pem file."; exit 1; }
	@crxmake --pack-extension=build/extension/ --extension-output=build/$(NAME)-$(VERSION).crx \
	  --pack-extension-key=conetserv.pem --ignore-file=chrome.manifest --ignore-file=config.xml \
	  --ignore-file=about.html --ignore-file=install.rdf --ignore-dir=firefox

_opera:
	@echo "  Build Opera widget.."
	@echo "   * build/$(NAME)-$(VERSION)-no-plugin.wgt"
	@$(RM) build/$(NAME)-$(VERSION)-no-plugin.wgt
	@cd build/extension && zip -q ../../build/$(NAME)-$(VERSION)-no-plugin.wgt \
	  config.xml \
	  `find js` `find images` `find css` index.html `find jquery`

.PHONY: firefox chrome
firefox: pre _firefox
chrome: pre _chrome
opera: pre _opera
