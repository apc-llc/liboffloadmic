#include <omp.h>
#include <stdio.h>

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

	#pragma omp parallel for
	for (int i = 0; i < args->n; i++)
		args->y[i] += args->a * args->x[i];

	#pragma omp parallel
	{
		#pragma omp master
		{
			printf("Using %d threads\n", omp_get_num_threads());
		}
	}
}

