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

  // (numProcesses-1)!, so overflows after 21 processes
  long long num = 1;

  if (myRank == 0) {
    MPI_Send(&num, 1, MPI_LONG_LONG_INT, 1, 42, MPI_COMM_WORLD);
    MPI_Recv(&num, 1, MPI_LONG_LONG_INT, numProcesses - 1, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Final result: %lld\n", num);
  } else {
    MPI_Recv(&num, 1, MPI_LONG_LONG_INT, myRank - 1, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Proc %d got %lld\n", myRank, num);
    num *= myRank;
    printf("Proc %d sending %lld\n", myRank, num);
    MPI_Send(&num, 1, MPI_LONG_LONG_INT, (myRank + 1) % numProcesses, 42, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
