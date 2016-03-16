#
# Build script for Linux workstation and
# Anselm cluster http://www.it4i.cz/
#

LOCATION = ..

include ../Makefile.inc

all: test

test: test.o kernel_embed_mic.o
	$(LOAD_GCC_MODULE) && $(HOST_CC) $(HOST_CFLAGS) $^ -o $@ -L$(INSTALL_PREFIX)/host/lib/$(TOOLEXECLIBDIR) -lmicrt -Wl,-rpath=$(INSTALL_PREFIX)/host/lib/$(TOOLEXECLIBDIR)

test.o: test.c ../install/host/include/mic_runtime.h
	$(LOAD_GCC_MODULE) && $(HOST_CC) $(HOST_CFLAGS) -std=c99 -I$(INSTALL_PREFIX)/host/include -c $< -o $@

kernel_embed_mic.o: kernel_embed.o
	objcopy --rename-section .data=.mic --keep-file-symbols $< $@

kernel_embed.o: kernel_embed.c kernel_embed.h
	$(LOAD_GCC_MODULE) && $(HOST_CC) $(HOST_CFLAGS) -I. -I$(INSTALL_PREFIX)/host/include -c $< -o $@

kernel_embed.h: libkernel.so
	xxd -include $< >$@

libkernel.so: kernel.c
	$(MIC_CC) $(MIC_CFLAGS) -std=c99 -fPIC -shared $< -o $@

ifeq (native,$(target))
run: test
	SINK_LD_LIBRARY_PATH=$(SINK_LD_LIBRARY_PATH):$(INSTALL_PREFIX)/target/lib/$(TOOLEXECLIBDIR) ./$<
else
run: test
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(INSTALL_PREFIX)/target/lib/$(TOOLEXECLIBDIR) ./$<
endif

clean:
	rm -rf *.o libkernel.so test kernel_embed.h

