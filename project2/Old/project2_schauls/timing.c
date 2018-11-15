#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#define	NUMBER_REPS 12
#define MSG_SIZE 8192
int main (int argc, char *argv[]){
  int                    /* number of samples per test */
    tag,                    /* MPI message tag parameter */
    numtasks,               /* number of MPI tasks */
    rank,                   /* my MPI task number */
    dest, source,          
    rc,                     /* return code */
    n;
  struct timeval T1, T2,T3, T4;
  int sumT,                   /* sum of all reps times */
    deltaT, deltaS,deltaR;    /* delta total, delta send, delta recv*/
 char msg[MSG_SIZE] = {0};             /* buffer containing 2MB message */
 MPI_Status status;          /* MPI receive routine parameter */
 struct timeval t1,t2;
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 MPI_Barrier(MPI_COMM_WORLD);
 sumT = 0;
 tag = 1;

 if (rank == 0) {
   dest = 1;
   source = 1;
   for (n = 1; n <= MSG_SIZE; n*=2) {
     printf("MSG SIZE: %d \n",n);
      gettimeofday(&T1, NULL);/* start time */
     
     
      rc = MPI_Send(&msg, n, MPI_BYTE, dest, tag, MPI_COMM_WORLD);
      if (rc != MPI_SUCCESS) {
         printf("Send error in task 0!\n");
         MPI_Abort(MPI_COMM_WORLD, rc);
         exit(1);
         }
      gettimeofday(&T3,NULL);
      deltaS = (T3.tv_sec - T1.tv_sec) * 1000000 + T3.tv_usec - T1.tv_usec;
      printf("send time: ");
      printf("%d\n", deltaS);
      /* Now wait to receive the echo reply from the worker  */
      /* If return code indicates error quit */
      gettimeofday(&T4, NULL);
      rc = MPI_Recv(&msg, n, MPI_BYTE, source, tag, MPI_COMM_WORLD,
                    &status);
      if (rc != MPI_SUCCESS) {
         printf("Receive error in task 0!\n");
         MPI_Abort(MPI_COMM_WORLD, rc);
         exit(1);
         }
      gettimeofday(&T2,NULL);
      deltaR = (T2.tv_sec - T4.tv_sec) * 1000000 + T2.tv_usec - T4.tv_usec;

      printf("Recv Time: ");

      printf("%d\n",deltaR);
      
      deltaT = deltaR - deltaS;

      printf("deltaT: %d\n",deltaT);
         sumT += deltaT;
	 printf("\n");
      }
   }

else if (rank == 1) {
   dest = 0;
   source = 0;
   for (n = 1; n <= MSG_SIZE; n*=2) {
      rc = MPI_Recv(&msg, n, MPI_BYTE, source, tag, MPI_COMM_WORLD,
                    &status);
      if (rc != MPI_SUCCESS) {
         printf("Receive error in task 1!\n");
         MPI_Abort(MPI_COMM_WORLD, rc);
         exit(1);
         }
      rc = MPI_Send(&msg, n, MPI_BYTE, dest, tag, MPI_COMM_WORLD);
      if (rc != MPI_SUCCESS) {
         printf("Send error in task 1!\n");
         MPI_Abort(MPI_COMM_WORLD, rc);
         exit(1);
         }
      }
   }

MPI_Finalize();
exit(0);
}
