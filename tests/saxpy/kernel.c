void saxpy(void* vargs)
{
	typedef struct
	{
		int n;
		float a;
		float* x;
		float* y;
	}
	Args;

	Args* args = (Args*)vargs;

	for (int i = 0; i < args->n; i++)
		args->y[i] += args->a * args->x[i];
}

