/**
 * pi_serial.c
 * Second part of asg2 which serially estimates the value of pi.
 * Author: Jared Klingenberger <klinge2@clemson.edu>
 * Date: January 28, 2015
 * I worked on this assignment alone, using only course materials.
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){

  int rank, size;
  int tmp;
  int tag;
  int i;

  MPI_Status status;

  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank != 1){
    if (rank == 0){
      tag = 1000;
    }
    else tag = rank;
    MPI_Send(&rank,1,MPI_INT,1,tag,MPI_COMM_WORLD);
  }
  if (rank == 1){
    for (i = 0; i < 3; i++){
      MPI_Recv(&tmp,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      printf("I receive value %d from process %d",tmp,status.MPI_SOURCE);
    }
  }

  printf("Hello Clemson from %d out of %d \n", rank, size);

  MPI_Finalize();
  return 0;
}
