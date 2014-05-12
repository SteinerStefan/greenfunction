	int numthreads = 32;
	#pragma omp parallel for num_threads(numthreads)
		for (i = 0; i < dim; i++)
		{ ...}