/**
 * pi_serial.c
 * Second part of asg2 which serially estimates the value of pi.
 * Author: Jared Klingenberger <klinge2@clemson.edu>
 * Date: January 28, 2015
 * I worked on this assignment alone, using only course materials.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DELTA 0.00001

typedef struct {
  double x, y;
} point;

/* Convert int rand() to a double in [0..1) */
double frand() {
  return (double)rand() / (double)RAND_MAX;
}

/* Calculate the distance between two points */
double dist(point a, point b) {
  double r; // radicand
  point diff; // b - a

  diff.x = b.x - a.x;
  diff.y = b.y - a.y;
  r = diff.y*diff.y + diff.x*diff.x;

  return sqrt(r);
}

/* Equivalent to dist between origin and p */
double mag(point p) {
  return sqrt(p.y*p.y + p.x*p.x);
}

int main(int argc, char *argv[]){
  
  srand(time(NULL));

  int n, i, hits=0;
  point zero;
  zero.x = 0.0f;
  zero.y = 0.0f;

  if (argc < 2) {
    fprintf(stderr, "syntax: %s <n points>\n", argv[0]);
    exit(1);
  } else
    n = atoi(argv[1]);

  for (i = 0; i < n; i++) {
    point p;
    p.x = frand();
    p.y = frand();

    /* Compare doubles */
    if (mag(p) - 1.0f < DELTA) {
      hits++;
    }
  }

  double pi = (double)hits/(double)n * 4.0f;
  printf("Hits in circle = %d\n", hits);
  printf("Hits in square = %d\n", n);
  printf("Estimated pi   = %f\n", pi);

  return 0;
}
