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
#include <assert.h>

#define N 30

double get_ms_to_send_bytes(int bytes) {
  MPI_Datatype MPI_BYTES;
  MPI_Type_contiguous(bytes, MPI_BYTE, &MPI_BYTES);
  MPI_Type_commit(&MPI_BYTES);

  int myRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  char *buf;
  buf = malloc(bytes * sizeof(char));

  double startTime = 0, endTime = 0;

  if (myRank == 1) {
    MPI_Send(buf, 1, MPI_BYTES, 0, 42, MPI_COMM_WORLD);
  } else {
    startTime = MPI_Wtime();
    MPI_Recv(buf, 1, MPI_BYTES, 1, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    endTime = MPI_Wtime();
  }

  free(buf);

  MPI_Barrier(MPI_COMM_WORLD);

  return (endTime - startTime) * 1000;
}

double get_throughput_in_MBps(int bytes) {
  double seconds = get_ms_to_send_bytes(bytes) / 1000;
  return bytes / (1024 * 1024 * seconds);
}

double avg(const double *measurements) {
  double min = 1e6, max = -1;
  int min_id = -1, max_id = -1;

  for (int i = 0; i < N; i++) {
    if (measurements[i] < min) {
      min = measurements[i];
      min_id = i;
    }
    if (measurements[i] > max) {
      max = measurements[i];
      max_id = i;
    }
  }

  double avg = 0;
  for (int i = 0; i < N; i++) if (i != min_id && i != max_id) {
    avg += measurements[i];
  }

  return avg / (N - 2);
}

int main(int argc, char * argv[])
{
  MPI_Init(&argc, &argv);

  assert(N > 2);

  int numProcesses, myRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
  assert(numProcesses == 2);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  double latency1[N], latency10[N], latency100[N], throughput[N];

  for (int i = 0; i < N; i++) {
    latency1[i] = get_ms_to_send_bytes(1);
    latency10[i] = get_ms_to_send_bytes(10);
    latency100[i] = get_ms_to_send_bytes(100);
    throughput[i] = get_throughput_in_MBps(100 * 1024 * 1024);
  }

  if (myRank == 0) {
    printf("avg 1 byte latency: %lf ms\n", avg(latency1));
    printf("avg 10 byte latency: %lf ms\n", avg(latency10));
    printf("avg 100 byte latency: %lf ms\n", avg(latency100));
    printf("avg 100MB throughput: %lf MB/s\n", avg(throughput));
  }

  MPI_Finalize();
  return 0;
}
