PACKAGE_NAME          := libPCM
CC                    := $(shell whereis cc)
CURDIR                := $(shell pwd)
CFLAGS                ?= -std=c11 -ferror-limit=1024 -Wall -pedantic
LDFLAGS               := -flto=thin
BUILD_DIR             ?= $(CURDIR)/BUILD
LIBBITIO_STATIC_LIB   := $(CURDIR)/Dependencies/BitIO/BUILD/libBitIO.a
LIBPCM_SOURCES        := $(CURDIR)/libPCM/src/%.c
LIBPCM_HEADERS        := $(CURDIR)/libPCM/include/%.h
LIBPCM_OBJECTS        := $(BUILD_DIR)/%.o
LIBPCM_STATIC_LIB     := $(BUILD_DIR)/libPCM.a
TRIM_UTIL_SRC         := $(CURDIR)/TrimSilence/TrimSilence.c
TRIM_UTIL             := $(BUILD_DIR)/TrimSilence

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

$(TRIM_UTIL): $(TRIM_UTIL_SRC) $(LIBPCM_STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ -c $(CURDIR)/TrimSilence/TrimSilence.c $(LDFLAGS) -L $(LIBBITIO_STATIC_LIB)

$(LIBPCM_OBJECTS): $(LIBPCM_SOURCES) $(LIBPCM_HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS) -L $(LIBBITIO_STATIC_LIB)

$(LIBPCM_STATIC_LIB): $(LIBPCM_SOURCES) $(LIBPCM_HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS) -L $(LIBBITIO_STATIC_LIB)
	ar -crsu $< $@
	ranlib -sf $@

all: $(LIBPCM_STATIC_LIB) $(TRIM_UTIL)
	cd $(CURDIR)/Dependencies/BitIO
	make -f Makefile
	cd ../..
	mkdir -p $(BUILD_DIR)
clean:
	cd $(BUILD_DIR)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
