#include <stdio.h>
#include <string.h>

const char * GOMP_OFFLOAD_get_name (void);
int GOMP_OFFLOAD_get_num_devices (void);
void GOMP_OFFLOAD_init_device (int device);
void * GOMP_OFFLOAD_alloc (int device, size_t size);
void GOMP_OFFLOAD_free (int device, void *tgt_ptr);
void * GOMP_OFFLOAD_host2dev (int device, void *tgt_ptr, const void *host_ptr, size_t size);
void * GOMP_OFFLOAD_dev2host (int device, void *host_ptr, const void *tgt_ptr, size_t size);

int main(int argc, char* argv[])
{
	const char* name = GOMP_OFFLOAD_get_name();
	printf("GOMP backend for %s\n", name);
	
	int num_devices = GOMP_OFFLOAD_get_num_devices();
	printf("%d %s device(s) available\n", num_devices, name);
	
	if (!num_devices) return 1;
	
	GOMP_OFFLOAD_init_device(0);
	
	const char* host_input = "Hello, device memory!";
	size_t szbuffer = strlen(host_input) + 1;
	char host_output[szbuffer];
	void* device_buffer = GOMP_OFFLOAD_alloc(0, szbuffer);
	
	GOMP_OFFLOAD_host2dev(0, device_buffer, host_input, szbuffer);
	GOMP_OFFLOAD_dev2host(0, host_output, device_buffer, szbuffer);
	
	printf("%s\n", host_output);
	
	GOMP_OFFLOAD_free(0, device_buffer);
	
	return 0;
}

