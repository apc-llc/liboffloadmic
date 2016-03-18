#
# Build script for Linux workstation and
# Anselm cluster http://www.it4i.cz/
#

LOCATION = ../..

include $(LOCATION)/Makefile.inc

all: test

ifeq (native,$(target))
../../tbb/build/linux_release/libtbb.so:
	cd ../../tbb/ && make offload=mic compiler=icc -j12 && cd build && ln -sf mic_icc_release linux_release
else
../../tbb/build/linux_release/libtbb.so:
	cd ../../tbb/ && make -j12 && cd build && ln -sf `find . -name "linux_intel64_gcc_*_release"` linux_release
endif

test: test.o kernel_embed_mic.o
	$(LOAD_GCC_MODULE) && $(HOST_CC) $(HOST_CFLAGS) $^ -o $@ -L$(INSTALL_PREFIX)/host/lib/$(TOOLEXECLIBDIR) -lmicrt -Wl,-rpath=$(INSTALL_PREFIX)/host/lib/$(TOOLEXECLIBDIR)

test.o: test.c $(LOCATION)/install/host/include/mic_runtime.h
	$(LOAD_GCC_MODULE) && $(HOST_CC) $(HOST_CFLAGS) -std=c99 -I$(INSTALL_PREFIX)/host/include -c $< -o $@

kernel_embed_mic.o: kernel_embed.o
	objcopy --rename-section .data=.mic --keep-file-symbols $< $@

kernel_embed.o: kernel_embed.c kernel_embed.h
	$(LOAD_GCC_MODULE) && $(HOST_CC) $(HOST_CFLAGS) -I. -I$(INSTALL_PREFIX)/host/include -c $< -o $@

kernel_embed.h: libkernel.so
	xxd -include $< >$@

libkernel.so: kernel.cpp ../../tbb/build/linux_release/libtbb.so
	$(MIC_CXX) $(MIC_CXXFLAGS) -fPIC -shared $< -o $@ -I../../tbb/include/ -L../../tbb/build/linux_release -ltbb -ltbbmalloc

ifeq (native,$(target))
run: test
	SINK_LD_LIBRARY_PATH=$(SINK_LD_LIBRARY_PATH):$(INSTALL_PREFIX)/target/lib/$(TOOLEXECLIBDIR) ./$<
else
run: test
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(INSTALL_PREFIX)/target/lib/$(TOOLEXECLIBDIR) ./$<
endif

clean:
	rm -rf *.o libkernel.so test kernel_embed.h

