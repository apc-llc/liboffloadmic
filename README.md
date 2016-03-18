### Standalone libgomp with MIC backend for explicit CUDA-like Xeon Phi device programming

This is a port of `libgomp` and `liboffloadmic` libraries for standalone use on Intel Xeon Phi K1OM devices. The codebase has been extracted from GCC 5.3.0 source tree. Central `Makefile` supports building for native Xeon Phi device or host emulation. As a prerequisite, building environment must have access to GCC crosscompiler for MIC (e.g. `/usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc` shipped with Intel MPSS).

### Features

CUDA-like API provided in `libmicrt` library enables direct user access to MIC devices enumeration, memory transfers and device kernels launching:

```
micError_t micGetPlatformName(const char** name);

micError_t micGetDeviceCount(int* count);

micError_t micSetDevice(int device);

micError_t micMalloc(void** devPtr, size_t size);

micError_t micFree(void* devPtr);

micError_t micMemcpy(void* dst, const void* src, size_t size, enum micMemcpyKind kind);

micError_t micLaunchKernel(const char *funcname, void *args);
```

### Building for Native Xeon Phi

```
$ git clone https://github.com/apc-llc/liboffloadmic.git
$ cd liboffloadmic
$ git submodule init
$ git submodule update
$ make target=native
```

### Building for Host Emulation

```
$ git clone https://github.com/apc-llc/liboffloadmic.git
$ cd liboffloadmic
$ git submodule init
$ git submodule update
$ make target=emul
```

### Testing

Host emulation:

```
$ cd liboffloadmic/test
$ make
$ make run
LD_LIBRARY_PATH=:/home/marcusmae/apc/liboffloadmic/test/../install/target/lib/../lib ./test
CUDA-like runtime API powered by GOMP backend for intelmic
1 intelmic device(s) available
num_threads = 8
Result is correct
```

Xeon Phi 5110P:

```
$ cd liboffloadmic/test
$ make
$ make run
SINK_LD_LIBRARY_PATH=:/home_lustre/dmikushin/liboffloadmic/tests/saxpy/../../install/target/lib/../lib64:/home_lustre/dmikushin/liboffloadmic/tests/saxpy/../../install/../tbb/build/linux_release ./test
CUDA-like runtime API powered by GOMP backend for intelmic
1 intelmic device(s) available
num_threads = 236
Result is correct
```

