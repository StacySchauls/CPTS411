#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

clock_t start, end, start2, end2;
double cpuTime;
double serial_times[10] = {0};
double parallel_times[10] = {0};

int generateInput(int n, int x){
  int i;
  time_t t;
  int A[n];
  srand((unsigned) time(&t));
  for(i = 0; i<n; i++){
    A[i] = rand()%100;
    //printf("Value at %d is: %d\n", i, A[i]);
  }




  //printf("Calling SerialSum...\n");
  start = clock();
  int serialSumVal = serialSum(A,n);
  end = clock();

  cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
  serial_times[x] = cpuTime;
  //printf("Serial at n = %d : %f\n", n, cpuTime);
  int par = parallelSumSimulator(A, n, n);

//  printf("Parallel sum is: %d\n----------------------------\n", par);
  return serialSumVal == par;
}

int serialSum(int numbers[], int n){


  int i, sum = 0;
  for(i = 0; i<n; i++){
    sum = sum + numbers[i];
  }
  return sum;
}


int parallelSumSimulator(int A[], int n, int p){
  int i;
  int t;
  int id;
  int t_lim = log2Help(n);
  start2 = clock();
  for(t = 0; t<t_lim; t++){
    for(id = 0; id < p; id+= 1 << t+1){
      A[id] = A[id] + A[(1<< t) + id];
    }
  }
  end2 = clock();
  cpuTime = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
  //printf("Parallel at n = %d : %f\n", n, cpuTime);

//printf("Parallel took %f seconds to execute.\n", cpuTime);
  return A[0];

//  printf("Global Sum from Parallel is: %d\n", global_sum);
}



int log2Help(int x){
  int log = -1;
  while(x){
    log++;
    x >>=1;
  }
  return log;
}

int testDriver(){
  printf("Running test driver:\n");
  int x1 = generateInput(8,0);
  int x2 = generateInput(16,1);
  int x3 = generateInput(32,2);
  int x4 = generateInput(64,3);
  int x5 = generateInput(128,4);
  int x6 = generateInput(256,5);
  int x7 = generateInput(1024,6);
  int x8 = generateInput(4096,7);
  int x9 = generateInput(8192,8);

  int i;
  for(i = 0; i<10; i++){
    //printf("%d : serial = %f : parallel = %f\n",i,serial_times[i],parallel_times[i]);
  }

  return(x1&&x2&&x3&&x4&&x5&&x6&&x7&&x8&&x9);

}


int main(char *argv, int argc){
  int n;
  //printf("Enter value of n...\n");
  //scanf("%d", &n);
  //generateInput(n);
  if(testDriver()){
    printf("All Sums correct!\n");
  }else{
    printf("Error in at least one sum\n");
  }
  return 0;
}
