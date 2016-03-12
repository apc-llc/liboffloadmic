### Standalone libgomp with MIC backend for explicit CUDA-like Xeon Phi device programming

This is a port of `libgomp` and `liboffloadmic` libraries for standalone use on Intel Xeon Phi K1OM devices. The codebase has been extracted from GCC 5.3.0 source tree. Central `Makefile` supports building for native Xeon Phi device or host emulation. As a prerequisite, building environment must have access to GCC crosscompiler for MIC (e.g. /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc shipped with Intel MPSS).

### Building for Native Xeon Phi

```
$ git clone https://github.com/apc-llc/liboffloadmic.git
$ cd liboffloadmic
$ make target=native
```

### Building for Host Emulation

```
$ git clone https://github.com/apc-llc/liboffloadmic.git
$ cd liboffloadmic
$ make target=emul
```

### Testing

```
$ cd liboffloadmic
$ make run

```
