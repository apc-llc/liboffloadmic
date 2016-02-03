```
$ mkdir build
$ cd build
$ ../configure --disable-option-checking --prefix=$(pwd)/../install --disable-multilib --enable-offload-targets=x86_64-intelmic-linux-gnu=/usr/linux-k1om-4.7/ --enable-languages=c,c++ --program-transform-name=s,y,y, --build=x86_64-pc-linux-gnu --host=x86_64-pc-linux-gnu --target=x86_64-pc-linux-gnu --enable-liboffloadmic=host build_alias=x86_64-pc-linux-gnu host_alias=x86_64-pc-linux-gnu target_alias=x86_64-pc-linux-gnu CXX="g++ -I$(pwd)/../runtime -I$(pwd)/../include/coi -g -O2 -D_GNU_SOURCE -DLINUX"
$ make ofldbegin.o ofldend.o
$ cd plugin
$ make main_target_image.h
```
