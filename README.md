### Build liboffloadmic

```
$ cd liboffloadmic
$ mkdir build_target
$ cd build_target
$ ../configure --disable-multilib --build=x86_64-intelmic-linux-gnu --host=x86_64-intelmic-linux-gnu --target=x86_64-intelmic-linux-gnu --enable-as-accelerator-for=x86_64-pc-linux-gnu --enable-liboffloadmic=target CXX="g++ -I$(pwd)/../runtime -I$(pwd)/../include/coi -g -O2 -D_GNU_SOURCE -DLINUX" --prefix=$(pwd)/../../install/target
$ make -j12
$ make install
$ cd ..
$ mkdir build_host
$ cd build_host
$ ../configure --disable-multilib --build=x86_64-pc-linux-gnu --host=x86_64-pc-linux-gnu --target=x86_64-pc-linux-gnu --enable-liboffloadmic=host CXX="g++ -I$(pwd)/../runtime -I$(pwd)/../include/coi -g -O2 -D_GNU_SOURCE -DLINUX" --prefix=$(pwd)/../../install/host --enable-offload-targets=x86_64-intelmic-linux-gnu=$(pwd)/../../install/target
$ make -j12
$ make install
$ cd ../..
```

## Build libgomp

```
$ cd libgomp
$ mkdir build_target
$ cd build_target
$ ../configure --disable-multilib --build=x86_64-intelmic-linux-gnu --host=x86_64-intelmic-linux-gnu --target=x86_64-intelmic-linux-gnu CXX="g++ -g -O2 -I$(pwd)/../../include -D_GNU_SOURCE -DLINUX" --prefix=$(pwd)/../../install/target
$ make -j12
$ make install
$ cd ..
$ mkdir build_host
$ cd build_host
$ ../configure --disable-multilib --build=x86_64-pc-linux-gnu --host=x86_64-pc-linux-gnu --target=x86_64-pc-linux-gnu CXX="g++ -g -O2 -D_GNU_SOURCE -DLINUX" --prefix=$(pwd)/../../install/host
$ make -j12
$ make install
$ cd ../..
```

