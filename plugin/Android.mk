# general settings
OS           = android
NDK_BASE     = ${HOME}/android/ndk
ANDROID_HOME = ${HOME}/android/mydroid
INCS         = -I$(ANDROID_HOME)/dalvik/libnativehelper/include/nativehelper \
		-I$(NDK_BASE)/include/bionic/arch-arm/include \
		-I$(NDK_BASE)/include/bionic/include \
		-I$(NDK_BASE)/include/kernel/include \
		-I$(NDK_BASE)/include/libm/include \
		-I$(NDK_BASE)/include/libm/include/arm \
		-I$(NDK_BASE)/include/libstdc++/include \
		-I$(ANDROID_HOME)/external/webkit/WebCore/bridge \
		-I$(ANDROID_HOME)/external/webkit/WebCore/plugins \
		-I$(ANDROID_HOME)/system/core/include \
		-I$(ANDROID_HOME)/frameworks/base/include

# flags
#CPPFLAGS += -DUNIX -DANDROID -DHAVE_CONFIG_H -D_DEBUG=1 -DAAPCS -D_GNU_SOURCE -DNOUNCRYPT
CPPFLAGS += -DUNIX -DANDROID -DHAVE_CONFIG_H -DAAPCS -D_GNU_SOURCE -DNOUNCRYPT
CFLAGS   += -Os -fno-exceptions -static -c -fpic ${INCS} ${CPPFLAGS}
EXPAT_CFLAGS += -DHAVE_MEMMOVE
LDFLAGS  = -fPIC -nostdlib -Wl,-soname,npconetserv.so -Wl,-shared,-Bsymbolic -shared -Wl,--gc-sections --Wl,--wrap,__aeabi_atexit
LIBS     = -L$(NDK_BASE)/lib -lc -lm -ldl
ALIB     = $(NDK_BASE)/toolchain/lib/gcc/arm-eabi/4.2.1/interwork/libgcc.a 
POSTLINK = -Wl,--no-undefined $(ALIB) 

# compiler and linker
AR    = $(NDK_BASE)/toolchain/bin/arm-eabi-ar
CC    = $(NDK_BASE)/toolchain/bin/arm-eabi-g++
LD    = $(NDK_BASE)/toolchain/bin/arm-eabi-ld
MAKE  = make

SRC = conetserv.c
OBJ += ${SRC:.c=.o}

all: options npconetserv.so

options:
	@echo "OS             = ${OS}"
	@echo "CC             = ${CC}"
	@echo "CFLAGS         = ${CFLAGS}"
	@echo "LDFLAGS        = ${LDFLAGS}"

%.o : %.c
	@echo CC $<
	@${CC} ${CFLAGS} -o $@ -c $<

npconetserv.so: ${OBJ}
	@echo LINK -o $@ ${LDFLAGS} $^ $(LIBS) ${POSTLINK}
	@${CC} -o $@ ${LDFLAGS} $^ $(LIBS) ${POSTLINK}

clean:
	rm -f npconetserv.so ${OBJ}

.PHONY: options all clean
