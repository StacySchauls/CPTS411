#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[])
{
  int i, count, n, p, seed;
  struct drand48_data drand_buf;
  double xp, yp;
  double startP, endP;

  n  = atoi(argv[1]);
  p = atoi(argv[2]);
  omp_set_num_threads (p);

 startP = omp_get_wtime();
  count = 0;
	printf("p = %d\n",p);
#pragma omp parallel private(i, xp, yp, seed, drand_buf) shared(n)
{
  seed = omp_get_thread_num() * 212711;
  srand48_r (seed, &drand_buf);

#pragma omp for reduction(+:count)
  for (i=0; i<n; i++) {
    drand48_r (&drand_buf, &xp);
    drand48_r (&drand_buf, &yp);
    if (xp*xp + yp*yp <= 1.0) count++;
  }
}

  endP = omp_get_wtime();
  printf("Estimate of pi: %15.10f\n", (double) ( 4.0*count) / (double) n);
  printf("Time: %7.2f\n", endP-startP);
}
