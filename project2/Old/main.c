#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

const int MAX_STRING = 100;
int main(int argc, char**argv){
  printf("here");
  int buf[2048] = {0};
  int num = 1;
  MPI_Init(NULL,NULL);
  char message[MAX_STRING];
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //while(num <= 2048){

    if(world_size < 2){
      printf("World must be > 1\n");
      MPI_Abort(MPI_COMM_WORLD,5);
    }

    if(world_rank == 0){
      strcpy(message, "Sedning message from 0 to 1");
      MPI_Send(message, strlen(message)+1,MPI_CHAR, 1,0,MPI_COMM_WORLD);
    }else if( world_rank == 1){
      printf("Message from process %d of %d\n", world_rank, world_size);
      MPI_Recv(message, MAX_STRING, MPI_CHAR,0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("got message from 0 \'%s\'\n", message);
    }

//  }
    MPI_Finalize();
    return 0;
}
