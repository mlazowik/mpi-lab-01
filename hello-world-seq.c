/*
 * A template for the 2016 MPI lab at the University of Warsaw.
 * Copyright (C) 2016, Konrad Iwanicki.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
  MPI_Init(&argc, &argv);
  unsigned t = rand() % 5;
  int64_t i=13;
  sleep(t);
  printf("Hello world from %d/%d (slept %u s)!\n", 0, 1, t);
  MPI_Finalize();
  return 0;
}
