#ifndef MICDEVRT_H
#define MICDEVRT_H

#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

// MIC error types
enum micError
{
	micSuccess                           =      0,
	micErrorMissingConfiguration         =      1,
	micErrorMemoryAllocation             =      2,
	micErrorInitializationError          =      3,
	micErrorLaunchFailure                =      4,
	micErrorPriorLaunchFailure           =      5,
	micErrorLaunchTimeout                =      6,
	micErrorLaunchOutOfResources         =      7,
	micErrorInvalidDeviceFunction        =      8,
	micErrorInvalidConfiguration         =      9,
	micErrorInvalidDevice                =     10,
	micErrorInvalidValue                 =     11,
	micErrorInvalidPitchValue            =     12,
	micErrorInvalidSymbol                =     13,
	micErrorMapBufferObjectFailed        =     14,
	micErrorUnmapBufferObjectFailed      =     15,
	micErrorInvalidHostPointer           =     16,
	micErrorInvalidDevicePointer         =     17,
	micErrorInvalidTexture               =     18,
	micErrorInvalidTextureBinding        =     19,
	micErrorInvalidChannelDescriptor     =     20,
	micErrorInvalidMemcpyDirection       =     21,
	micErrorAddressOfConstant            =     22,
	micErrorTextureFetchFailed           =     23,
	micErrorTextureNotBound              =     24,
	micErrorSynchronizationError         =     25,
	micErrorInvalidFilterSetting         =     26,
	micErrorInvalidNormSetting           =     27,
	micErrorMixedDeviceExecution         =     28,
	micErrorCudartUnloading              =     29,
	micErrorUnknown                      =     30,
	micErrorNotYetImplemented            =     31,
	micErrorMemoryValueTooLarge          =     32,
	micErrorInvalidResourceHandle        =     33,
	micErrorNotReady                     =     34,
	micErrorInsufficientDriver           =     35,
	micErrorSetOnActiveProcess           =     36,
	micErrorInvalidSurface               =     37,
	micErrorNoDevice                     =     38,
	micErrorECCUncorrectable             =     39,
	micErrorSharedObjectSymbolNotFound   =     40,
	micErrorSharedObjectInitFailed       =     41,
	micErrorUnsupportedLimit             =     42,
	micErrorDuplicateVariableName        =     43,
	micErrorDuplicateTextureName         =     44,
	micErrorDuplicateSurfaceName         =     45,
	micErrorDevicesUnavailable           =     46,
	micErrorInvalidKernelImage           =     47,
	micErrorNoKernelImageForDevice       =     48,
	micErrorIncompatibleDriverContext    =     49,
	micErrorPeerAccessAlreadyEnabled     =     50,
	micErrorPeerAccessNotEnabled         =     51,
	micErrorDeviceAlreadyInUse           =     54,
	micErrorProfilerDisabled             =     55,
	micErrorProfilerNotInitialized       =     56,
	micErrorProfilerAlreadyStarted       =     57,
	micErrorProfilerAlreadyStopped        =    58,
	micErrorAssert                        =    59,
	micErrorTooManyPeers                 =     60,
	micErrorHostMemoryAlreadyRegistered  =     61,
	micErrorHostMemoryNotRegistered      =     62,
	micErrorOperatingSystem              =     63,
	micErrorPeerAccessUnsupported        =     64,
	micErrorLaunchMaxDepthExceeded       =     65,
	micErrorLaunchFileScopedTex          =     66,
	micErrorLaunchFileScopedSurf         =     67,
	micErrorSyncDepthExceeded            =     68,
	micErrorLaunchPendingCountExceeded   =     69,
	micErrorNotPermitted                 =     70,
	micErrorNotSupported                 =     71,
	micErrorHardwareStackError           =     72,
	micErrorIllegalInstruction           =     73,
	micErrorMisalignedAddress            =     74,
	micErrorInvalidAddressSpace          =     75,
	micErrorInvalidPc                    =     76,
	micErrorIllegalAddress               =     77,
	micErrorInvalidPtx                   =     78,
	micErrorInvalidGraphicsContext       =     79,
	micErrorStartupFailure               =   0x7f,
	micErrorApiFailureBase               =  10000
};

typedef enum micError micError_t;

// Return the name of the accelerator platform
micError_t micGetPlatformName(const char** name);

micError_t micGetDeviceCount(int* count);

micError_t micSetDevice(int device);

micError_t micMalloc(void** devPtr, size_t size);

micError_t micMallocAligned(void** devPtr, size_t size, size_t alignment);

micError_t micFree(void* devPtr);

micError_t micFreeAligned(void* devPtr);

enum micMemcpyKind
{
	micMemcpyHostToDevice = 0,
	micMemcpyDeviceToHost
};

micError_t micMemcpy(void* dst, const void* src, size_t size, enum micMemcpyKind kind);

// Load the specified ELF image containing device functions.
micError_t micRegisterModule(unsigned char* image, size_t size);

struct dim3
{
    unsigned int x, y, z;
#if defined(__cplusplus)
    dim3(unsigned int vx = 1, unsigned int vy = 1, unsigned int vz = 1) : x(vx), y(vy), z(vz) {}
#endif /* __cplusplus */
};

typedef struct dim3 dim3;

micError_t micLaunchKernel(const char *funcname, void *args);

micError_t micDeviceSynchronize();

micError_t micGetSymbolAddress(void** devPtr, const char* symbol);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif // MICDEVRT_H

