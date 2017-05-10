# call make with ARCH=Architecture (Architecture being either x86_64 or arm64), TYPE=BuildType (BuildType being either debug or release).
# clean cleans the generated files (objects, libraries, and executables)

#.POSIX:
PACKAGE_NAME          := libPCM
CC                    := $(shell whereis cc)
CURDIR                := $(shell pwd)
CFLAGS                ?= -std=c11 -ferror-limit=1024 -Wall -pedantic -Wno-unknown-pragmas
LDFLAGS               := -flto=thin
BUILD_DIR             ?= $(CURDIR)/BUILD
LIBBITIO_STATIC_LIB   := $(CURDIR)/Dependencies/BitIO/BUILD/libBitIO.a
LIBPCM_STATIC_LIB     := $(BUILD_DIR)/libPCM.a
LIBPCM_SOURCE_DIR     ?= $(CURDIR)/libPCM/src
LIBPCM_SOURCES        ?= $(shell find ./libPCM/src -iname *.c)
LIBPCM_HEADER_DIR     ?= $(CURDIR)/libPCM/include
LIBPCM_HEADERS        ?= $(shell find ./libPCM/include -iname *.h)
LIBPCM_OBJECTS        ?= $(BUILD_DIR)/%.o

.DEFAULT_GOAL := all

.PHONY: all ARCH TYPE check distclean CheckVer test install clean uninstall

ifeq (${ARCH}, arm64)
	$(BUILD_DIR)  += /arm64
	$(CFLAGS)     += -march=arm64
elif ($(ARCH),x86_64)
	$(BUILD_DIR)  += /x86_64
	$(CFLAGS)     += -march=x86_64
else
	$(CFLAGS)     += -march=native
endif

ifeq (${TYPE}, debug)
	$(CFLAGS)     += -DDEBUG -g -O0
else
	$(CFLAGS)     += -DNODEBUG -fvectorize -loop-vectorize -funroll-loops -Ofast
endif

all: $(LIBPCM_STATIC_LIB) $(BUILD_DIR)/TrimSilence
	cd $(CURDIR)/Dependencies/BitIO
	make -f Makefile
	cd ../..
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/TrimSilence: $(CURDIR)/TrimSilence/TrimSilence.c $(LIBPCM_STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ -c $? $(LDFLAGS) -L$(LIBBITIO_STATIC_LIB)

$(BUILD_DIR)/%.o: $(LIBPCM_SOURCES)
	$(CC) $(CFLAGS) -c $? $(LDFLAGS) -L$(LIBBITIO_STATIC_LIB)

$(LIBPCM_STATIC_LIB): $(BUILD_DIR)/%.o
	ar -crsu $? $@
	ranlib -sf $@
clean:
	cd $(BUILD_DIR)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
