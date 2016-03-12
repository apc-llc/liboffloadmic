#include <mic_runtime.h>
#include <pthread.h>
#include <vector>

using namespace std;

// As in CUDA, different devices can be binded
// to different host threads.
static __thread int currentDevice = 0;

static vector<bool> initialized;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

extern "C"
{
	const char * GOMP_OFFLOAD_get_name (void);
	int GOMP_OFFLOAD_get_num_devices (void);
	void GOMP_OFFLOAD_init_device (int device);
	void * GOMP_OFFLOAD_alloc (int device, size_t size);
	void GOMP_OFFLOAD_free (int device, void *tgt_ptr);
	void * GOMP_OFFLOAD_host2dev (int device, void *tgt_ptr, const void *host_ptr, size_t size);
	void * GOMP_OFFLOAD_dev2host (int device, void *host_ptr, const void *tgt_ptr, size_t size);

	static inline void initialize()
	{
		pthread_mutex_lock(&lock);
		if (!initialized[currentDevice])
		{
			GOMP_OFFLOAD_init_device(currentDevice);
			initialized[currentDevice] = true;
		}
		pthread_mutex_unlock(&lock);		
	}

	// Return the name of the accelerator platform
	micError_t micGetPlatformName(const char** name)
	{
		if (!name)
			return micErrorInvalidHostPointer;
		
		*name = GOMP_OFFLOAD_get_name();

		return micSuccess;
	}

	micError_t micGetDeviceCount(int* count)
	{
		if (!count)
			return micErrorInvalidHostPointer;
	
		*count = GOMP_OFFLOAD_get_num_devices();

		return micSuccess;
	}

	micError_t micSetDevice(int device)
	{
		if (device >= initialized.size())
			return micErrorInvalidDevice;

		currentDevice = device;

		return micSuccess;
	}

	micError_t micMalloc(void** devPtr, size_t size)
	{
		initialize();

		if (!devPtr)
			return micErrorInvalidDevicePointer;

		*devPtr = GOMP_OFFLOAD_alloc(currentDevice, size);
		
		if (!devPtr)
			return micErrorInvalidDevicePointer;

		return micSuccess;
	}

	micError_t micFree(void* devPtr)
	{
		if (!initialized[currentDevice])
			return micErrorInitializationError;

		if (!devPtr)
			return micErrorInvalidDevicePointer;
	
		GOMP_OFFLOAD_free(currentDevice, devPtr);

		return micSuccess;
	}

	micError_t micMemcpy(void* dst, const void* src, size_t size, micMemcpyKind kind)
	{
		initialize();
		
		if (kind == micMemcpyHostToDevice)
		{
			if (!dst)
				return micErrorInvalidDevicePointer;
			if (!src)
				return micErrorInvalidHostPointer;
				
			GOMP_OFFLOAD_host2dev(currentDevice, dst, src, size);
		}
		else
		{
			if (!dst)
				return micErrorInvalidHostPointer;
			if (!src)
				return micErrorInvalidDevicePointer;
			
			GOMP_OFFLOAD_dev2host(currentDevice, dst, src, size);
		}
		
		return micSuccess;
	}
}

namespace
{
	class Init
	{
	public :
		Init()
		{
			int count = 0;
			micGetDeviceCount(&count);
			initialized.resize(count);
		}
	};
	
	static Init init;
}

