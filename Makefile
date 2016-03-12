#
# Build script for Linux workstation and
# Anselm cluster http://www.it4i.cz/
#

.PHONY: run

INSTALL_PREFIX := $(shell pwd)/install

HOST_CC := gcc
HOST_CXX := g++

MIC_PREFIX := /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-
MIC_CC := $(MIC_PREFIX)gcc
MIC_CXX := $(MIC_PREFIX)g++

HOST_CFLAGS := -I$(shell pwd)/libgomp/runtime -I$(shell pwd)/libgomp/include/coi -I$(shell pwd)/liboffloadmic/build_target/plugin/ -g -O0 -D_GNU_SOURCE -DLINUX
HOST_CXXFLAGS := $(HOST_CFLAGS)

TARGET_LIB_PATH := $(INSTALL_PREFIX)/target/lib/$(shell $(MIC_CC) -print-multi-os-directory)
MIC_LIB := $(TARGET_LIB_PATH) -lintrinsics
MIC_CFLAGS := $(HOST_CFLAGS) -I$(INSTALL_PREFIX)/target/lib/gcc/k1om-intelmic-linux-gnu/5.3.0/include -L$(MIC_LIB) -D__MIC__
MIC_CXXFLAGS := $(MIC_CFLAGS)

# Check we are on Anselm. Otherwise, locally installed Xeon Phi is expected.
ANSELM := $(shell echo $(PS1) | grep -oh anselm)

ifneq (,$(ANSELM))
LOAD_GCC_MODULE := module load gcc/4.9.0
LOAD_ICC_MODULE := module load intel
endif

all: install/host/lib64/liboffloadmic_host.so

$(TARGET_LIB_PATH)/libintrinsics.a: intrinsics/build_target/intrinsics.o
	$(LOAD_GCC_MODULE) && mkdir -p $(TARGET_LIB_PATH) && ar rcs $@ $<

intrinsics/build_target/intrinsics.o: intrinsics/intrinsics.c
	$(LOAD_ICC_MODULE) && mkdir -p intrinsics/build_target && icc -mmic -c $< -o $@

install/host/lib64/liboffload_host.so: $(TARGET_LIB_PATH)/libintrinsics.a
	$(LOAD_GCC_MODULE) && cd libgomp && \
	mkdir -p build_target && \
	cd build_target && \
	../configure --disable-multilib --build=x86_64-intelmic-linux-gnu --host=k1om-intelmic-linux-gnu --target=k1om-intelmic-linux-gnu CC="$(MIC_CC) $(MIC_CFLAGS)" CXX="$(MIC_CXX) $(MIC_CXXFLAGS)" --prefix=$(INSTALL_PREFIX)/target && \
	make -j12 && \
	make install && \
	cd .. && \
	mkdir -p build_host && \
	cd build_host && \
	../configure --disable-multilib --build=x86_64-pc-linux-gnu --host=x86_64-pc-linux-gnu --target=x86_64-pc-linux-gnu CC="$(HOST_CC) $(HOST_CFLAGS)" CXX="$(HOST_CXX) $(HOST_CXXFLAGS)" --prefix=$(INSTALL_PREFIX)/host && \
	make -j12 && \
	make install && \
	cd ../.. && \
	cd liboffloadmic && \
	mkdir -p build_target && \
	cd build_target && \
	../configure --disable-multilib --build=x86_64-intelmic-linux-gnu --host=k1om-intelmic-linux-gnu --target=x86_64-intelmic-linux-gnu --enable-as-accelerator-for=x86_64-pc-linux-gnu --enable-liboffloadmic=target CC="$(MIC_CC) $(MIC_CFLAGS)" CXX="$(MIC_CXX) $(MIC_CXXFLAGS)" --prefix=$(INSTALL_PREFIX)/target && \
	make -j12 && \
	make install && \
	cd .. && \
	rm -rf ../install/target/lib64/libcoi_device* && \
	mkdir -p build_host && \
	cd build_host && \
	../configure --disable-multilib --build=x86_64-pc-linux-gnu --host=x86_64-pc-linux-gnu --target=x86_64-pc-linux-gnu --enable-liboffloadmic=host CC="$(HOST_CC) $(HOST_CFLAGS)" CXX="$(HOST_CXX) $(HOST_CXXFLAGS)" --prefix=$(INSTALL_PREFIX)/host --enable-offload-targets=x86_64-intelmic-linux-gnu=$(INSTALL_PREFIX)/host && \
	make -j12 && \
	make install && \
	cd ../.. && \
	rm -rf install/host/lib64/libcoi_host*

TOOLEXECLIBDIR := $(shell gcc -print-multi-os-directory)

test/test: test/test.c install/host/lib64/liboffloadmic_host.so
	$(LOADMODULES_LOGIN) gcc -g -Iinstall/host/include $< -o $@ -Linstall/host/lib/$(TOOLEXECLIBDIR) -lgomp-plugin-intelmic -loffloadmic_host -lpthread -Wl,-rpath=$(shell pwd)/install/host/lib/$(TOOLEXECLIBDIR)

run: test/test
	SINK_LD_LIBRARY_PATH=$(shell pwd)/install/target/lib/$(TOOLEXECLIBDIR) ./$<

clean:
	rm -rf install && \
	rm -rf intrinsics/build_target && \
	rm -rf liboffloadmic/build_target && \
	rm -rf liboffloadmic/build_host && \
	rm -rf libgomp/build_target && \
	rm -rf libgomp/build_host && \
	rm -rf test/test

