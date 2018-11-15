#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int p,tid,seed;
long long int i;
long long int n;
long double count;
long double d,piEst,xp,yp;
double ratio;

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("Need 2 arguents, n and p\n");
    exit(-1);
  }

  n = atoll(argv[1]);
  p = atoi(argv[2]);
 // printf("n is %ld\n",n);
/******PARALLEL********/
double startP = omp_get_wtime();
#pragma omp_num_threads(p);
#pragma omp parallel for private(xp,yp,d) shared(n,p)  reduction(+:count)
    for(i = 0; i < n; i++){

      tid = omp_get_thread_num();
      seed = tid + 1;
      seed = seed * i;
      xp = (double) rand_r(&seed) /( double) RAND_MAX;
      yp = (double) rand_r(&seed) /( double) RAND_MAX;
      d = sqrt((xp*xp) + (yp*yp));
      if(d<=1){
        count++;
      }
    }

double endP = omp_get_wtime();
double totalP = endP - startP;
  ratio =  count /(double) n;
//  printf("count: %f\nn: %ld\nratio:%f\n",count, n, ratio);
  piEst = ratio * 4.0;
  printf("Estimation: %Lf, was done in %f\n",piEst, time_spent_parallel);
/*
  *******SERIAL*********
  count = 0;
  double startS = omp_get_wtime();  
  for(i = 0; i < n; i++){
      seed = 97 +(7*i);
      seed = seed * i;
      xp = (double) rand_r(&seed) /( double) RAND_MAX;
      yp = (double) rand_r(&seed) /( double) RAND_MAX;
      d = sqrt((xp*xp) + (yp*yp));
      if(d<=1){
        count++;
      }
    }

double endS = omp_get_wtime();
double totalS = endS - startS;
  ratio =  count /(double) n;
 // printf("count: %f\nn: %ld\nratio:%f\n",count, n, ratio);
  piEst = ratio * 4.0;
  printf("Estimation: %Lf, was done in %f\n",piEst, totalS);
*/
  return 0;
}
