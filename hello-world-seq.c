/*
 * A template for the 2016 MPI lab at the University of Warsaw.
 * Copyright (C) 2016, Konrad Iwanicki.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

int main(int argc, char * argv[])
{
  MPI_Init(&argc, &argv);

  struct timeval start;
  gettimeofday(&start, NULL);
  srand(start.tv_usec);

  int numProcesses, myRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  if (myRank == 0) {
    printf("Process 0 reporting number of processes: %d\n", numProcesses);
  }

  unsigned t = rand() % 5;
  sleep(t);
  printf("Hello world from %d/%d (slept %u s)!\n", myRank, 1, t);
  MPI_Finalize();
  return 0;
}
