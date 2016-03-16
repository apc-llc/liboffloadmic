#include <libgomp/libgomp-plugin.h> // addr_pair
#include <map>
#include <mic_runtime.h>
#include <pthread.h>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

// As in CUDA, different devices can be binded
// to different host threads.
static __thread int currentDevice = 0;

static bool initialized = false;

// Tables of symbols addresses for each device.
static vector<map<string, void*> >* symtabs;

// Locks to access symbols tables of single device
// from multiple threads.
static vector<pthread_mutex_t>* locks;

extern "C"
{
	__attribute__((constructor)) static void initialize()
	{
		void GOMP_OFFLOAD_init_device (int device);

		// Must be static locals here, otherwise constructors
		// for global statics could be called later than this
		// function call, unexpectedly reinitializing the variables.
		static vector<map<string, void*> > _symtabs;
		symtabs = &_symtabs;
		static vector<pthread_mutex_t> _locks;
		locks = &_locks;

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

			// Set sizes for symbols tables and locks.
			symtabs->resize(count);
			locks->resize(count);
			for (int i = 0; i < count; i++)
				pthread_mutex_init(&(*locks)[i], NULL);

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

	// Load the specified ELF image containing device functions.
	micError_t micRegisterModule(unsigned char* image, size_t size)
	{
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
		GOMP_OFFLOAD_load_image(currentDevice, im_start_end, &table);
		
		return micSuccess;
	}

	micError_t micLaunchKernel(const char *funcname, void *args)
	{
		void GOMP_OFFLOAD_run (int device, void *tgt_fn, void *tgt_vars);

		if (!funcname)
			return micErrorInvalidSymbol;

		void* func = NULL;
		micGetSymbolAddress(&func, funcname);
		
		// TODO cache resolved functions in table
		
		GOMP_OFFLOAD_run (currentDevice, func, args);
		
		return micSuccess;
	}

	micError_t micDeviceSynchronize()
	{
		// TODO
	}

	micError_t micGetSymbolAddress(void** devPtr, const char* symbol)
	{		
		void* __offload_get_symbol_address(int device, const char* name);

		if (!devPtr)
			return micErrorInvalidHostPointer;

		if (!symbol || !strcmp(symbol, ""))
			return micErrorInvalidSymbol;

		pthread_mutex_lock(&(*locks)[currentDevice]);

		map<string, void*>::iterator i = (*symtabs)[currentDevice].find(symbol);
		if (i != (*symtabs)[currentDevice].end())
			*devPtr = i->second;
		else
		{
			*devPtr = __offload_get_symbol_address(currentDevice, symbol);
			(*symtabs)[currentDevice][symbol] = *devPtr;
		}

		pthread_mutex_unlock(&(*locks)[currentDevice]);
		
		return micSuccess;
	}
}

