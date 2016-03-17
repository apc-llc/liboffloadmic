#include <omp.h>
#include <stdio.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

using namespace tbb;

struct saxpy_func
{
	float a;
	float* x;
	float* y;

	saxpy_func(float a_, float* x_, float* y_) : a(a_), x(x_), y(y_) { }

	void operator()(const blocked_range<int>& range) const
	{
		for (int i = range.begin(); i != range.end(); i++)
			y[i] += a * x[i];
	}
};

extern "C" void saxpy(void* vargs)
{
	static task_scheduler_init init;

	typedef struct
	{
		int n;
		float a;
		float* x;
		float* y;
	}
	Args;

	Args* args = (Args*)vargs;

	saxpy_func func(args->a, args->x, args->y);
	parallel_for(blocked_range<int>(0, args->n), func);
}

