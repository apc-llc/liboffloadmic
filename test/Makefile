all: test

test: test.c
	gcc -g -I. $< -o $@ -I../libgomp -I../libgomp/build_host -I../libgomp/config/posix -L../install/host/lib -lgomp-plugin-intelmic -lpthread

run: test
	LD_PRELOAD=/opt/intel/mic/coi/host-linux-release/lib/libcoi_host.so.0 ./$<

clean:
	rm -rf test

