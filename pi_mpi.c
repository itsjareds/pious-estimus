/**
 * pi_mpi.c
 * Second part of asg2 which estimates the value of pi using MPI.
 * Author: Jared Klingenberger <klinge2@clemson.edu>
 * Date: January 28, 2015
 * I worked on this assignment alone, using only course materials.
*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

#define DELTA 0.0000001f

typedef struct {
  double x, y;
} point;

/* Convert int rand() to a double in [0..1) */
double frand() {
  return (double)rand() / (double)RAND_MAX;
}

/* Equivalent to dist between origin and p */
double mag(point p) {
  return sqrt(p.y*p.y + p.x*p.x);
}

/* Just in case e.g. sqrt(7) returns 6.999995 */
int isqrt(double f) {
  return (int)(sqrt(f) + 0.5f);
}

int main(int argc, char *argv[]){

  int rank, size;
  int tmp;
  int tag;
  int i;
  double start, stop, elapsed;
  point origin;
  double s; // length of one side of plot
  double origArr[2];
  long npoints, n, hits = 0;

  if (argc < 2) {
    fprintf(stderr, "Syntax: %s <n points>\n", argv[0]);
    exit(1);
  } else {
    npoints = strtol(argv[1], NULL, 10);
  }

  MPI_Status status;

  MPI_Init(&argc,&argv);
  start = MPI_Wtime();

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  srand(rank + time(NULL));
  n = npoints / size;
  if (rank == 0 && n * size < npoints)
    n += npoints - n * size;
  s = 2.0f/sqrt(size);

  /** Send out plot section data 
    * Probably more optimal to do something like a tree barrier...
    */
  if (rank == 0) {
    origin.x = -1.0f;
    origin.y = -1.0f;
    for (i = 1; i < size; i++) {
      origArr[0] = -1.0f + (i % isqrt(size)) * s;
      origArr[1] = -1.0f + (i / isqrt(size)) * s;
      MPI_Send(&origArr, 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&origArr, 2, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    origin.x = origArr[0];
    origin.y = origArr[1];
  }

  /* Start computing delicious homemade pi */
  //printf("[%d/%d] origin = (%lf, %lf)\n", rank, size, origin.x, origin.y);

  long l;
  for (l = 0; l < n; l++) {
    point p;
    p.x = origin.x + frand() * s/2.0f;
    p.y = origin.y + frand() * s/2.0f; // negative since starting from top left
    //printf("[%d/%d] p = (%lf, %lf)\n", rank, size, p.x, p.y);
    //break;

    // Compare doubles
    if (mag(p) - 1.0f < DELTA) {
      hits++;
    }
  }

  if (rank != 0) {
    long stats[2] = {hits, l};
    MPI_Send(&stats, 2, MPI_LONG, 0, rank, MPI_COMM_WORLD);
  } else {
    for (i = 1; i < size; i++) {
      long tmp[2];
      MPI_Recv(&tmp, 2, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      hits += tmp[0];
      l += tmp[1];
    }

    /* After receiving the last result */

    stop = MPI_Wtime();
    elapsed = stop - start;

    long total = l;
    double pi = (double)hits / (double) (total) * 4.0f;

    printf("Hits in circle: %ld\n", hits);
    printf("Hits in square: %ld\n", total);
    printf("Pi estimation:  %lf\n", pi);

    printf("Time: %lf sec\n", elapsed);
  }

  MPI_Finalize();
  return 0;
}
