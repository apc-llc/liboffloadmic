#include <kernel_embed.h>
#include <mic_runtime.h>

__attribute__((constructor)) static void kernel_o_register(void)
{
	micRegisterModule(libkernel_so, libkernel_so_len);
}

