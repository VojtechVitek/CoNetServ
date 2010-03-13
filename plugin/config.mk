include ../config.mk

# ARCH-specific settings (x86: -m32; x86_64: -m64)
ARCH = -m64

# OS-specific settings
ifeq (${shell uname}, Darwin)
OSCFLAGS = -DVERSION=\"${VERSION}\" -DWEBKIT_DARWIN_SDK
OSLDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
else
OSINCS = -I/usr/include/xulrunner-1.9.1.8/stable/
OSCFLAGS = -DVERSION=\"${VERSION}\" -DXULRUNNER_SDK
OSLDFLAGS = -L/usr/lib -lc
endif

CFLAGS = -g -std=gnu99 --pedantic -Wall -fPIC ${ARCH} ${OSINCS} ${OSCFLAGS}
LDFLAGS = ${ARCH} ${OSLDFLAGS}

CC = gcc
