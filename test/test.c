#include <stdio.h>

const char * GOMP_OFFLOAD_get_name (void);

int GOMP_OFFLOAD_get_num_devices (void);

int main(int argc, char* argv[])
{
	const char* name = GOMP_OFFLOAD_get_name();
	printf("GOMP backend for %s\n", name);
	
	int num_devices = GOMP_OFFLOAD_get_num_devices();
	printf("%d %s device(s) available\n", num_devices, name);
	
	if (!num_devices) return 1;
	
	return 0;
}

