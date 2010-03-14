# CoNetServ - Complex Network Services
# NPAPI plugin for browser extensions

include config.mk

TEST = test.html
SRC = conetserv.c conetserv_posix.c idna.c punycode.c convert_utf.c stringprep.c nfkc.c profiles.c
OBJ = ${SRC:.c=.o}

all: options npconetserv.so ${shell uname}

options:
	@echo conetserv build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

npconetserv.so: ${OBJ}
	@echo LD $@
	@${CC} -v -shared -o $@ ${OBJ} ${LDFLAGS}

debug: ${OBJ}
	@echo CC debug.c
	@${CC} -c ${CFLAGS} -o debug.o debug.c
	@echo LD debug
	${CC} -o debug debug.o ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f debug.o debug
	@rm -f npconetserv.so ${OBJ} Localized.rsrc
	@rm -rf npconetserv.plugin

Linux:
	@chmod 755 npconetserv.so
	@echo Setup: sudo ln -s ${shell pwd}/npconetserv.so /usr/lib/mozilla/plugins/npconetserv.so

Darwin:
	/Developer/Tools/Rez -o Localized.rsrc -useDF Localized.r
	mkdir -p npconetserv.plugin/Contents/MacOS
	mkdir -p npconetserv.plugin/Contents/Resources/English.lproj
	cp -r Localized.rsrc npconetserv.plugin/Contents/Resources/English.lproj
	cp -f Info.plist npconetserv.plugin/Contents
	cp -f npconetserv.so npconetserv.plugin/Contents/MacOS/npconetserv
	@echo Setup: sudo ln -s `pwd`/npconetserv.so /Library/Internet\\ Plug-Ins/npconetserv.plugin

.PHONY: all clean debug options Darwin Linux
