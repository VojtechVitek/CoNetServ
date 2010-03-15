# ARCH-specific settings
#ARCH = -m32 #x86
#ARCH = -m64 #x86_64

# Debug mode on/off
DEBUG = -g # on
#DEBUG = -o2 # off


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

CFLAGS = -std=gnu99 --pedantic -Wall -fPIC \
	 ${DEBUG} ${ARCH} ${OSINCS} ${OSCFLAGS} \
         -DVERSION=\"${VERSION}\" -DBUILDDATE=\"$(BUILDDATE)\"

LDFLAGS = ${ARCH} ${OSLDFLAGS}

CC = gcc
