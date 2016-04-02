#
# Build script for Linux workstation and
# Anselm cluster http://www.it4i.cz/
#

.PHONY: run

LOCATION ?= .

INSTALL_PREFIX ?= $(shell pwd)/$(LOCATION)/install

#
# Select native or host-emulation build mode.
# By default, if mic0 is found locally, build for native Xeon Phi,
# otherwise - build for host emulation.
#
target ?= undefined
REMOVE_LIBCOI_DEVICE := true
REMOVE_LIBCOI_HOST := true
ifeq (undefined,$(target))
ifeq (server is down,$(shell ping -c1 -W1 mic0 2>/dev/null || echo 'server is down' | grep "server is down"))
target := emulation
else
target := native
REMOVE_LIBCOI_DEVICE := rm -rf $(INSTALL_PREFIX)/mic/lib64/libcoi_device*
REMOVE_LIBCOI_HOST := rm -rf $(INSTALL_PREFIX)/host/lib64/libcoi_host*
endif
endif

HOST_CC := gcc
HOST_CXX := g++

ifeq (native,$(target))
MIC_PREFIX ?= /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-
MIC_CC := $(MIC_PREFIX)gcc
MIC_CXX := $(MIC_PREFIX)g++
else
MIC_CC := $(HOST_CC)
MIC_CXX := $(HOST_CXX)
endif

HOST_LIB_PATH := $(INSTALL_PREFIX)/host/lib/$(shell $(MIC_CC) -print-multi-os-directory)
HOST_CFLAGS := -I$(shell pwd)/$(LOCATION)/libgomp/runtime -I$(shell pwd)/$(LOCATION)/libgomp/include/coi -I$(shell pwd)/$(LOCATION)/liboffloadmic/build_mic/plugin/ -g -O3 -gdwarf-3 -D_GNU_SOURCE -DLINUX -DGET_DEVICE_SYMBOL_ADDRESS_SUPPORT
HOST_CXXFLAGS := $(HOST_CFLAGS)

TARGET_LIB_PATH := $(INSTALL_PREFIX)/mic/lib/$(shell $(MIC_CC) -print-multi-os-directory)
ifeq (native,$(target))
MIC_LIB := $(TARGET_LIB_PATH) -lintrinsics
MIC_CFLAGS := $(HOST_CFLAGS) -I$(INSTALL_PREFIX)/mic/lib/gcc/k1om-intelmic-linux-gnu/5.3.0/include -L$(MIC_LIB) -D__MIC__
MIC_CXXFLAGS := $(MIC_CFLAGS)
else
MIC_LIB := $(TARGET_LIB_PATH)
MIC_CFLAGS := $(HOST_CFLAGS) -L$(MIC_LIB)
MIC_CXXFLAGS := $(MIC_CFLAGS)
endif

# Check we are on Anselm. Otherwise, locally installed Xeon Phi is expected.
ANSELM := $(shell cat /etc/profile.d/prompt.sh | grep -oh anselm)

LOAD_GCC_MODULE := true
LOAD_ICC_MODULE := true
ifneq (,$(ANSELM))
LOAD_GCC_MODULE := module unload intel && module unload gcc && module load gcc/4.9.0 && module load binutils
LOAD_ICC_MODULE := module unload gcc && module load intel
endif

DEPENDS_ON_INTRINSICS_OR_NOT :=
ifeq (native,$(target))
DEPENDS_ON_INTRINSICS_OR_NOT := $(TARGET_LIB_PATH)/libintrinsics.a
endif

LIBOFFLOADMIC_HOST := $(INSTALL_PREFIX)/host/lib/$(shell $(MIC_CC) -print-multi-os-directory)/liboffloadmic_host.so

TOOLEXECLIBDIR := $(shell gcc -print-multi-os-directory)

