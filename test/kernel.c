void saxpy(float a, float* x, float* y, int n)
{
	for (int i = 0; i < n; i++)
		y[i] += a * x[i];
}

void target_register_lib (const void *);

void *__offload_func_table[1]
	__attribute__ ((__used__, visibility ("hidden"),
	section (".gnu.offload_funcs"))) = { &saxpy };

void *__offload_var_table[0]
	__attribute__ ((__used__, visibility ("hidden"),
	section (".gnu.offload_vars"))) = { };

void *__OFFLOAD_TARGET_TABLE__[]
	__attribute__ ((__used__, visibility ("hidden"))) =
	{
		&__offload_func_table, &__offload_func_table + 1,
		&__offload_var_table, &__offload_var_table
	};

__attribute__((constructor)) static void register_lib(void)
{
	target_register_lib (__OFFLOAD_TARGET_TABLE__);
}

