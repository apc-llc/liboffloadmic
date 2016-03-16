#include <libgomp/libgomp-plugin.h> // addr_pair
#include <mic_runtime.h>
#include <pthread.h>

// As in CUDA, different devices can be binded
// to different host threads.
static __thread int currentDevice = 0;

static bool initialized = false;

extern "C"
{
	__attribute__((constructor)) static void initialize()
	{
		void GOMP_OFFLOAD_init_device (int device);

		static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

		pthread_mutex_lock(&lock);
		if (!initialized)
		{
			int count = 0;
			micGetDeviceCount(&count);

			// Initialized all devices at once, because offload_image()
			// sets up image on all devices, presuming they are initialized.
			for (int i = 0; i < count; i++)
				GOMP_OFFLOAD_init_device(i);

			initialized = true;
		}
		pthread_mutex_unlock(&lock);		
	}

	// Return the name of the accelerator platform
	micError_t micGetPlatformName(const char** name)
	{
		const char * GOMP_OFFLOAD_get_name (void);

		if (!name)
			return micErrorInvalidHostPointer;
		
		*name = GOMP_OFFLOAD_get_name();

		return micSuccess;
	}

	micError_t micGetDeviceCount(int* count)
	{
		int GOMP_OFFLOAD_get_num_devices (void);

		if (!count)
			return micErrorInvalidHostPointer;
	
		*count = GOMP_OFFLOAD_get_num_devices();

		return micSuccess;
	}

	micError_t micSetDevice(int device)
	{
		int count = 0;
		micGetDeviceCount(&count);

		if (device >= count)
			return micErrorInvalidDevice;

		currentDevice = device;

		return micSuccess;
	}

	micError_t micMalloc(void** devPtr, size_t size)
	{
		void * GOMP_OFFLOAD_alloc (int device, size_t size);

		if (!devPtr)
			return micErrorInvalidDevicePointer;

		*devPtr = GOMP_OFFLOAD_alloc(currentDevice, size);
		
		if (!devPtr)
			return micErrorInvalidDevicePointer;

		return micSuccess;
	}

	micError_t micFree(void* devPtr)
	{
		void GOMP_OFFLOAD_free (int device, void *tgt_ptr);

		if (!devPtr)
			return micErrorInvalidDevicePointer;
	
		GOMP_OFFLOAD_free(currentDevice, devPtr);

		return micSuccess;
	}

	micError_t micMemcpy(void* dst, const void* src, size_t size, micMemcpyKind kind)
	{
		void * GOMP_OFFLOAD_host2dev (int device, void *tgt_ptr, const void *host_ptr, size_t size);
		void * GOMP_OFFLOAD_dev2host (int device, void *host_ptr, const void *tgt_ptr, size_t size);

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

static void* func = NULL;
	
	// Load the specified ELF image containing device functions.
	micError_t micRegisterModule(unsigned char* image, size_t size)
	{
		void * GOMP_OFFLOAD_host2dev (int device, void *tgt_ptr, const void *host_ptr, size_t size);
		int GOMP_OFFLOAD_load_image (int device, void *target_image, addr_pair **result);

		if (!image)
			return micErrorInvalidValue;
	
		// Could be called earlier than constructor-attributed initialized()
		// function, because kernels register themselves also with
		// constructor attribute.
		if (!initialized)
			initialize();
	
		addr_pair *table = NULL;
		void* im_start_end[2];
		im_start_end[0] = (void*)image;
		im_start_end[1] = (void*)(image + size);
		int sztable = GOMP_OFFLOAD_load_image(currentDevice, im_start_end, &table);
		
		for (int i = 0; i < sztable / sizeof(void*); i++)
			printf("%p %p\n", (void*)(table[i].start), (void*)(table[i].end));
		
		func = (void*)(table[0].start);
		
		return micSuccess;
	}

	micError_t micLaunchKernel(const char *funcname, /*dim3 gridDim, dim3 blockDim,*/ void *args)
	{
		void GOMP_OFFLOAD_run (int device, void *tgt_fn, void *tgt_vars);

		if (!func)
			return micErrorInvalidDeviceFunction;
		
		GOMP_OFFLOAD_run (currentDevice, func, args);
		
		/*if ((gridDim.x == 0) || (gridDim.y == 0) || (gridDim.z == 0))
			return micErrorInvalidConfiguration;

		if ((blockDim.x == 0) || (blockDim.y == 0) || (blockDim.z == 0))
			return micErrorInvalidConfiguration;*/
		
		// TODO Load target image with the requested function.
		//addr_pair *result = NULL;
		//GOMP_OFFLOAD_load_image(currentDevice, image, &result);

		// micErrorLaunchFailure
		// micErrorLaunchTimeout
		// micErrorLaunchOutOfResources
		// micErrorSharedObjectInitFailed
		
		return micSuccess;
	}

	micError_t micDeviceSynchronize()
	{
		// TODO
	}
}

