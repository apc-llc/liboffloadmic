```
$ mkdir build
$ cd build
$ ../configure --disable-multilib --build=x86_64-intelmic-linux-gnu --host=x86_64-intelmic-linux-gnu --target=x86_64-intelmic-linux-gnu --enable-as-accelerator-for=x86_64-pc-linux-gnu --enable-liboffloadmic=target CXX="g++ -I/home/marcusmae/apc/liboffloadmic/liboffloadmic/build_target/../runtime -I/home/marcusmae/apc/liboffloadmic/liboffloadmic/build_target/../include/coi -g -O2 -D_GNU_SOURCE -DLINUX" --prefix=/home/marcusmae/apc/liboffloadmic/liboffloadmic/build_target/../install/target
$ make -j12
```
