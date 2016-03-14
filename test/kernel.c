void saxpy(float a, float* x, float* y, int n)
{
	for (int i = 0; i < n; i++)
		y[i] += a * x[i];
}

