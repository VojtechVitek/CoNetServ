VERSION = 1.0

ifeq (${shell uname}, Darwin)
CPPFLAGS = -DVERSION=\"${VERSION}\" -DWEBKIT_DARWIN_SDK
LDFLAGS = -dynamiclib -fPIC #-framework Carbon -framework CoreFoundation -framework WebKit
else
INCS = -I/usr/include/xulrunner-1.9.1.7/stable/
CPPFLAGS = -DVERSION=\"${VERSION}\" -DXULRUNNER_SDK
#LDFLAGS = -L/usr/lib -lc
endif
CFLAGS = -g -pedantic -Wall -O2 -fPIC ${INCS} ${CPPFLAGS}

CC = cc
