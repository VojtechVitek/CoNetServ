# ARCH-specific settings (x86: -m32; x86_64: -m64)
ARCH = -m32

# OS-specific settings
ifeq (${shell uname}, Darwin)
OSCFLAGS = -DWEBKIT_DARWIN_SDK
OSLDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
else
OSINCS = -I/usr/include/xulrunner-1.9.1.8/stable/
OSCFLAGS = -DXULRUNNER_SDK
OSLDFLAGS = -lc
endif

include ../VERSION

CFLAGS = -g -std=gnu99 --pedantic -Wall -fPIC \
	 ${ARCH} ${OSINCS} ${OSCFLAGS} \
         -DVERSION=\"${VERSION}\" -DBUILDDATE=\"$(BUILDDATE)\"

LDFLAGS = ${ARCH} ${OSLDFLAGS}

CC = gcc
