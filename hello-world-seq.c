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

  unsigned t = rand() % 5;
  sleep(t);
  printf("Hello world from %d/%d (slept %u s)!\n", myRank, 1, t);

  MPI_Datatype MPI_PAIR;
  MPI_Type_contiguous(2, MPI_INT, &MPI_PAIR);
  MPI_Type_commit(&MPI_PAIR);

  if (myRank == 0) {
    printf("\tProcess 0 reporting number of processes: %d\n", numProcesses);

    for (int i = 1; i < numProcesses; ++i) {
      int pair[2];
      MPI_Recv(pair, 1, MPI_PAIR, i, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      printf("\t(%d, %d)\n", pair[0], pair[1]);
    }
  } else {
    int pair[2];
    pair[0] = myRank;
    pair[1] = rand() % 11;
    printf("\tMy (rank %d) random number: %d\n", myRank, pair[1]);

    MPI_Send(pair, 1, MPI_PAIR, 0, 42, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
