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
	
	const char* host_input = "Hello, device memory!";
	size_t szbuffer = strlen(host_input) + 1;
	char host_output[szbuffer];
	
	void* device_buffer;
	MIC_ERROR_CHECK(micMalloc(&device_buffer, szbuffer));
	
	MIC_ERROR_CHECK(micMemcpy(device_buffer, host_input, szbuffer, micMemcpyHostToDevice));
	MIC_ERROR_CHECK(micMemcpy(host_output, device_buffer, szbuffer, micMemcpyDeviceToHost));
	
	printf("%s\n", host_output);

	MIC_ERROR_CHECK(micFree(device_buffer));
	
	return 0;
}

