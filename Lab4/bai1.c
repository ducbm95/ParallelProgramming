#include <mpi.h>
#include <stdio.h>

#define N 1000 // pi accuracy

void master() {
  int i;
  double pi = 0, temp;
  for (i = 0; i < N; i++) {
    MPI_Recv(&temp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0,
      MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    pi += temp;
  }

  printf("Pi = %f\n", pi * 4);
}

void slave(int rank, int procs) {
  int i;
  for (i = 0; i < N; i++) {
    if (rank == i % procs + 1) {
      double temp;
      temp = (i % 2 == 0) ? 1.0 / (2.0 * i + 1) : -1.0 / (2.0 * i + 1);
      MPI_Send(&temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
  }
}

double findPi() {
  int i;
  double pi = 0;
  for (i = 0; i < N; i++) {
    double temp;
    temp = (i % 2 == 0) ? 1.0 / (2.0 * i + 1) : -1.0 / (2.0 * i + 1);
    pi += temp;
  }
  printf("Pi = %f\n", pi * 4);
  return pi;
}

void main(int argc, char** argv) {
  int rank, size;
  double start, end, t1, t2;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  start = MPI_Wtime();
  if (rank == 0)
    master();
  else
    slave(rank, size - 1);
  end = MPI_Wtime();
  t1 = end - start;


  if (rank == 0) {
    // for sequence programing
    start = MPI_Wtime();
    findPi();
    end = MPI_Wtime();
    t2 = end - start;

    printf("Parallel time: %f\n", t1);
    printf("Sequence time: %f\n", t2);
    printf("Speed up: %f\n", t2 / t1);
    printf("Efficency: %f\n", t2 / t1 / size);
  }

  MPI_Finalize();

}
