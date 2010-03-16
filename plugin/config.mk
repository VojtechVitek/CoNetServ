ARCH = x86_64
DEBUG = on

#############################

KERNEL=${shell uname}

ifeq (${KERNEL},Darwin)
OSCFLAGS = -DWEBKIT_DARWIN_SDK
OSLDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
else
OSINCS = -I/usr/include/xulrunner-1.9.1.8/stable/
OSCFLAGS = -DXULRUNNER_SDK
OSLDFLAGS = -lc
endif

include ../VERSION

ifeq (${ARCH},x86_64)
ARCHFLAGS = -m64
else
ifeq (${ARCH},x86)
ARCHFLAGS = -m32
else
${error "INVALID ARCHITECTURE"}
endif
endif

ifeq (${DEBUG},off)
DEBUG = -O2 -DNDEBUG
else
DEBUG = -g
endif

CFLAGS = -std=gnu99 --pedantic -Wall -fPIC \
	 ${DEBUGFLAGS} ${ARCHFLAGS} ${OSINCS} ${OSCFLAGS} \
         -DVERSION=\"${VERSION}\" -DBUILDDATE=\"$(BUILDDATE)\"

LDFLAGS = ${ARCHFLAGS} ${OSLDFLAGS}

CC = gcc
