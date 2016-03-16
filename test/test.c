#include <malloc.h>
#include <mic_runtime.h>
#include <stdlib.h>
#include <string.h>

#define MIC_ERROR_CHECK(x) do { micError_t err = x; if (( err ) != micSuccess ) { \
	printf ("Error %d at %s :%d \n" , err, __FILE__ , __LINE__ ) ; exit(-1);\
}} while (0)

int main(int argc, char* argv[])
{
	const char* name;
	MIC_ERROR_CHECK(micGetPlatformName(&name));
	printf("CUDA-like runtime API powered by GOMP backend for %s\n", name);
	
	int num_devices;
	MIC_ERROR_CHECK(micGetDeviceCount(&num_devices));
	printf("%d %s device(s) available\n", num_devices, name);
	
	if (!num_devices) return 1;

	double dinvrandmax = 1.0 / (double)RAND_MAX;

	typedef struct
	{
		int n;
		float a;
		float* x;
		float* y;
	}
	Args;

	Args host_args;
	
	host_args.n = 1000;
	host_args.a = rand() * dinvrandmax;
	float* host_x = (float*)malloc(host_args.n * sizeof(float));
	float* host_y = (float*)malloc(host_args.n * sizeof(float));
	for (int i = 0; i < host_args.n; i++)
	{
		host_x[i] = rand() * dinvrandmax;
		host_y[i] = rand() * dinvrandmax;
	}
	float* device_x;
	MIC_ERROR_CHECK(micMalloc((void**)&device_x, host_args.n * sizeof(float)));
	MIC_ERROR_CHECK(micMemcpy(device_x, host_x, host_args.n * sizeof(float), micMemcpyHostToDevice));
	host_args.x = device_x;
	float* device_y;
	MIC_ERROR_CHECK(micMalloc((void**)&device_y, host_args.n * sizeof(float)));
	MIC_ERROR_CHECK(micMemcpy(device_y, host_y, host_args.n * sizeof(float), micMemcpyHostToDevice));
	host_args.y = device_y;

	Args* device_args;
	MIC_ERROR_CHECK(micMalloc((void**)&device_args, sizeof(Args)));
	MIC_ERROR_CHECK(micMemcpy(device_args, &host_args, sizeof(Args), micMemcpyHostToDevice));

	MIC_ERROR_CHECK(micLaunchKernel("saxpy", device_args));
	MIC_ERROR_CHECK(micDeviceSynchronize());

	float* host_y_result = (float*)malloc(host_args.n * sizeof(float));
	MIC_ERROR_CHECK(micMemcpy(host_y_result, device_y, host_args.n * sizeof(float), micMemcpyDeviceToHost));
	
	// Check results.
	int mismatch = 0;
	for (int i = 0; i < host_args.n; i++)
		if (host_y_result[i] != host_args.a * host_x[i] + host_y[i])
		{
			printf("Results mismatch @ i = %d: %f != %f\n", i,
				host_y_result[i], host_args.a * host_x[i] + host_y[i]);
			mismatch = 1;
			break;
		}
	if (!mismatch)
		printf("Result is correct\n");
	
	MIC_ERROR_CHECK(micFree(device_x));
	MIC_ERROR_CHECK(micFree(device_y));
	MIC_ERROR_CHECK(micFree(device_args));

	free(host_x);
	free(host_y);
	free(host_y_result);
	
	return 0;
}

