# CoNetServ - Complex Network Services
# NPAPI plugin for browser extensions

include config.mk

SRC = conetserv.c conetserv_posix.c
OBJ = ${SRC:.c=.o}

.PHONY: all clean debug options
all: npconetserv.so

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

options:
	@echo "CoNetServ plugin build options:"
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

npconetserv.so: ${OBJ}
	@echo LD $@
	@${CC} -shared -o $@ ${OBJ} ${LDFLAGS}
	@if [ "$$(uname)" == "Darwin" ]; then \
	  echo "CP npconetserv.so ../build/${KERNEL}_${ARCH}-gcc3/plugins/npCoNetServ.plugin/*"; \
	  /Developer/Tools/Rez -o Localized.rsrc -useDF Localized.r; \
	  mkdir -p npCoNetServ.plugin/Contents/MacOS; \
	  mkdir -p npCoNetServ.plugin/Contents/Resources/English.lproj; \
	  cp -r Localized.rsrc npCoNetServ.plugin/Contents/Resources/English.lproj; \
	  cp -f Info.plist npCoNetServ.plugin/Contents; \
	  cp -f npconetserv.so npCoNetServ.plugin/Contents/MacOS/npCoNetServ; \
	  mkdir -p ../build/${KERNEL}_${ARCH}-gcc3/plugins/; \
	  cp npCoNetServ.plugin ../build/${KERNEL}_${ARCH}-gcc3/plugins/; \
	elif [ "$$(uname)" == "Linux" ]; then \
	  echo "CP npconetserv.so ../build/${KERNEL}_${ARCH}-gcc3/plugins/npCoNetServ*.so"; \
	  chmod 755 npconetserv.so; \
	  mkdir -p ../build/${KERNEL}_${ARCH}-gcc3/plugins/; \
	  cp npconetserv.so ../build/${KERNEL}_${ARCH}-gcc3/plugins/npCoNetServ_${KERNEL}_${ARCH}-gcc3.so; \
	fi


debug: ${OBJ}
	@echo CC debug.c
	@${CC} -c ${CFLAGS} -o debug.o debug.c
	@echo LD debug
	@${CC} -o debug debug.o ${OBJ} ${LDFLAGS}

clean:
	@echo Cleaning..
	@rm -f debug.o debug
	@rm -f npconetserv.so ${OBJ} Localized.rsrc
	@rm -rf npCoNetServ.plugin

