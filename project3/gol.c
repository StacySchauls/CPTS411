#include <stddef.h>
#include <getopt.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdio.h>

const int DEAD = 0;
const int ALIVE = 1;
const int BIG_PRIME = 93377;
//the data that willbe used for the instace of life
struct CELL {
  int rank;           //what rank it belongs to
  int life_value;     //dead = 0 alive = 1
  struct CELL *N, *S,*E,*W,*NE,*NW,*SE,*SW; //neighbors
  //int location[][];       //location on the grid
};



//function prototypes
int generateInitialGoL();
int simulate();
int displayGoL();
int randNum();
int simulate();
int determineState();
//global variables
int n, G;     //#rows and #col  //# of generatioons
int p, pid;   //number of processors and process id
//struct CELL matrix[n][n];

int main(int argc, char **argv){
  if(argc < 3){
    printf("Not enough arguments.\n");
    exit(-1);
  }
  n = atoi(argv[1]);
  G = atoi(argv[2]);
  // printf("We have %d rows/col and %d generations\n",n,G);

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD,&pid);

  //printf("Number of proc: %d\n", p);

  if(!generateInitialGoL()){
    printf("init success\n");
    simulate();
  }else
    printf("init failed\n");
    MPI_Finalize();



    return 0;
}

int generateInitialGoL(){
  int rNum, i;

  if(pid == 0){
    //printf("rank 0. \nGenerating random numbers between 1 and 93377\n");
    srand(time(NULL));
    for(i = 0; i<p; i++){
      rNum = randNum();
      MPI_Send(&rNum,1, MPI_INT, i, 0, MPI_COMM_WORLD);
      //printf("sent %d to process %d\n",rNum, i);
    }

  }else{
    int j;
    int sequence[(n*n)/p];
    struct CELL localRows[n][n/p];
    MPI_Recv(&rNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //printf("recvieved %d from process 0\n",rNum);
     srand(rNum);
     printf("here. n : %d p = %d",n,p);
     for(j = 0; j<(n/p); j++){
       printf("j : %d\n",j);
       for(i = 0; i<n; i++){
         printf("i: ")
         sequence[i] = randNum();

         if(sequence[i]%2 == 0){
   	       printf("cell at [%d][%d} is alive",i,j);
	       localRows[j][i].life_value = ALIVE;

         }else{
           localRows[j][i].life_value = DEAD;
           printf("cell at [%d][%d} is dead",i,j);
         }
         localRows[j][i].rank = pid;

         //printf("process %d generated %d\n", pid, rNum);
       }
     }





  }

  return 0;
}

int randNum(){
   int i;
   i = (rand() % BIG_PRIME) + 1;
   // printf("Returning: %d\n",i);
 return i;
}

int simulate(){
  int i;
  printf("In simulate. pid is %d\n", pid);
  for(i = 0; i<G; i++){

  }
  return 0;
}

int determineState(){

  return 0;
}
