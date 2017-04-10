PACKAGE_NAME        := libPCM
CC                  := cc
CFLAGS              := -std=c11 -march=native -Ofast -funroll-loops -ferror-limit=1024 -Wall -pedantic
LDFLAGS             := -flto=thin
BUILD_DIR           := $(CURDIR)/BUILD
LIBPCM_SOURCE_DIR   := $(CURDIR)/libPCM/src
LIBPCM_HEADER_DIR   := $(CURDIR)/libPCM/include
LIBPCM_OBJECT_DIR   := $(BUILD_DIR)/libPCM

LIBPCM_SOURCE_FILES := \
$(LIBPCM_SOURCE_DIR)/libPCM.c \
$(LIBPCM_SOURCE_DIR)/Decode/AIFDecoder.c \
$(LIBPCM_SOURCE_DIR)/Decode/W64Decoder.c \
$(LIBPCM_SOURCE_DIR)/Decode/WAVDecoder.c \
$(LIBPCM_SOURCE_DIR)/Encode/AIFEncoder.c \
$(LIBPCM_SOURCE_DIR)/Encode/W64Encoder.c \
$(LIBPCM_SOURCE_DIR)/Encode/WAVEncoder.c

LIBPCM_HEADER_FILES := \
$(LIBPCM_HEADER_DIR)/libPCM.h \
$(LIBPCM_HEADER_DIR)/PCMTypes.h \
$(LIBPCM_HEADER_DIR)/AIFCommon.h \
$(LIBPCM_HEADER_DIR)/W64Common.h \
$(LIBPCM_HEADER_DIR)/WAVCommon.h

LIBPCM_OBJECT_FILES := \
$(LIBPCM_OBJECT_DIR)/libPCM.a \
$(LIBPCM_OBJECT_DIR)/libPCM.o \
$(LIBPCM_OBJECT_DIR)/AIFDecoder.o \
$(LIBPCM_OBJECT_DIR)/W64Decoder.o \
$(LIBPCM_OBJECT_DIR)/WAVDecoder.o \
$(LIBPCM_OBJECT_DIR)/AIFEncoder.o \
$(LIBPCM_OBJECT_DIR)/W64Encoder.o \
$(LIBPCM_OBJECT_DIR)/WAVEncoder.o

LIBPCM_STATIC_LIBRARY := $(LIBPCM_OBJECT_DIR)/libPCM.a($(LIBPCM_OBJECT_FILES))

.PHONY: all check distclean CheckVer release debug test install clean uninstall

$(LIBPCM_OBJECT_FILES): $(LIBPCM_SOURCE_FILES) $(LIBPCM_HEADER_FILES)
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

$(LIBPCM_STATIC_LIBRARY): $(LIBPCM_OBJECT_FILES)
	ar -crsu $< $@
	ranlib -sf $@

all:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIBPCM_OBJECT_DIR)
	$(LIBPCM_STATIC_LIBRARY)

release:
	$(CFLAGS) += -DNODEBUG -Ofast -flto=thin
	$(all)
debug:
	$(CFLAGS) += -DDEBUG -g -O0
	$(all)
clean:
	cd $(LIBPCM_OBJECT_DIR)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
