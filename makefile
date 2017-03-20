PACKAGE_NAME        := libPCM
VERSION             := $(shell grep @version $(CURDIR)/libPCM/include/libPCM.h | echo | grep -o '[0-9]\.[0-9]\.[0-9]')
CC                  := cc
DESTINATION         := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR           := $(CURDIR)/BUILD
CFLAGS              := -std=c11 -march=native -funroll-loops -lmath `pkg-config --libs libBitIO`
LDFLAGS             := -flto=thin
DEB_ERROR_OPTIONS   := -Wno-unused-parameter -Wno-unused-variable -Wno-int-conversion
REL_ERROR_OPTIONS   := -Weverything -Wunreachable-code -Wno-conversion
DEB_FLAGS           := $(CFLAGS) -g -o0 $(DEB_ERROR_OPTIONS) $(LDFLAGS)
SANITIZER           := -fsanitize=undefined -fsanitize=address
REL_FLAGS           := $(CFLAGS) -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)
BUILD_LIB           := $(BUILD_DIR)/libPCM

.PHONY: all check distclean CheckVer release debug test install clean uninstall

all: release
	$(release)
check: test
	$(test)
distclean: clean
	$(clean)
CheckVer:
	$(shell echo ${VERSION})
release: $(CURDIR)/libPCM/src/libPCM.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libPCM/src/libPCM.c -o $(BUILD_LIB)/libPCM.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libPCM/src/Decode/AIFDecoder.c -o $(BUILD_LIB)/AIFDecoder.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libPCM/src/Decode/W64Decoder.c -o $(BUILD_LIB)/W64Decoder.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libPCM/src/Decode/WAVDecoder.c -o $(BUILD_LIB)/WAVDecoder.o
	ar -crsu   $(BUILD_LIB)/libPCM.a $(BUILD_LIB)/*.o
	ranlib -sf $(BUILD_LIB)/libPCM.a
	strip	 $(BUILD_LIB)/libPCM.a
debug: $(CURDIR)/libPCM/src/libPCM.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libPCM/src/libPCM.c -o $(BUILD_LIB)/libPCM.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libPCM/src/Decode/AIFDecoder.c -o $(BUILD_LIB)/AIFDecoder.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libPCM/src/Decode/W64Decoder.c -o $(BUILD_LIB)/W64Decoder.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libPCM/src/Decode/WAVDecoder.c -o $(BUILD_LIB)/WAVDecoder.o
	ar -crsu   $(BUILD_LIB)/libPCM.a $(BUILD_LIB)/*.o
	ranlib -sf $(BUILD_LIB)/libPCM.a
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_LIB)/libPCM.a $(DESTINATION)/lib/libPCM.a
	install -C -v -m 444 $(CURDIR)/libPCM/include/libPCM.h $(DESTINATION)/include/libPCM.h
	install -C -v -m 444 $(CURDIR)/libPCM.pc /usr/share/pkgconfig/libPCM.pc
clean:
	cd $(BUILD_LIB)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
uninstall:
	rm -d -i $(DESTINATION)
