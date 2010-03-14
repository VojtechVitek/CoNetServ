include ../config.mk

# ARCH-specific settings (x86: -m32; x86_64: -m64)
ARCH = -m32 

# OS-specific settings
ifeq (${shell uname}, Darwin)
CPPFLAGS = -DVERSION=\"${VERSION}\" -DWEBKIT_DARWIN_SDK
LDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
else
INCS = -I/usr/include/xulrunner-1.9.1.8/stable/
CPPFLAGS = -DVERSION=\"${VERSION}\" -DXULRUNNER_SDK
#LDFLAGS = -L/usr/lib -lc
endif
CFLAGS = -g -std=gnu99 --pedantic -Wall -O2 -fPIC ${INCS} ${CPPFLAGS} 
#-m32

CC = gcc
