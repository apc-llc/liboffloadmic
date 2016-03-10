#include <stdio.h>

void atomic_thread_fence_int (int memorder)
{
	__atomic_thread_fence(memorder);
}

unsigned int atomic_load_n_unsigned_int (unsigned int *ptr, int memorder)
{
	return __atomic_load_n(ptr, memorder);
}

long atomic_load_n_long (long *ptr, int memorder)
{
	return __atomic_load_n(ptr, memorder);
}

unsigned long long atomic_load_n_unsigned_long_long (unsigned long long *ptr, int memorder)
{
	return __atomic_load_n(ptr, memorder);
}

size_t atomic_load_n_size_t (size_t *ptr, int memorder)
{
	return __atomic_load_n(ptr, memorder);
}

struct gomp_task;

struct gomp_task* atomic_load_n_gomp_task (struct gomp_task **ptr, int memorder)
{
	return __atomic_load_n(ptr, memorder);
}

void atomic_store_n_unsigned_int (unsigned int *ptr, unsigned int val, int memorder)
{
	__atomic_store_n(ptr, val, memorder);
}

void atomic_store_n_gomp_task (struct gomp_task **ptr, struct gomp_task *val, int memorder)
{
	__atomic_store_n(ptr, val, memorder);
}

void atomic_store_n_size_t (size_t *ptr, size_t val, int memorder)
{
	__atomic_store_n(ptr, val, memorder);
}

