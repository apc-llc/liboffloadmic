#
# Run "make" on login node.
# Run "make run" on allocated compute node.
#

# Modules to load on Anselm's login node and allocated compute node
LOADMODULES_LOGIN :=
LOADMODULES_ALLOC :=

# Check we are on Anselm. Otherwise, locally installed Xeon Phi is expected.
ANSELM := $(shell echo $(PS1) | grep -oh anselm)

ifneq (,$(ANSELM))
LOADMODULES_LOGIN := module load GCCcore/4.9.3 && module load binutils/2.25-GCCcore-4.9.3 &&
LOADMODULES_ALLOC := module load gcc/4.9.0 &&
endif

TOOLEXECLIBDIR := $(shell gcc -print-multi-os-directory)

all: test

test: test.c
	$(LOADMODULES_LOGIN) gcc -g -I. $< -o $@ -I../libgomp -I../libgomp/build_hos -I../libgomp/config/posix -L../install/host/lib/$(TOOLEXECLIBDIR) -lgomp-plugin-intelmic -loffloadmic_host -lpthread -Wl,-rpath=`pwd`/../install/host/lib/$(TOOLEXECLIBDIR)

run: test
	$(LOADMODULES_ALLOC) LD_PRELOAD=/opt/intel/mic/coi/host-linux-release/lib/libcoi_host.so.0 LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):`pwd`/../install/target/lib/../lib ./$<

clean:
	rm -rf test

