#include <mpi.h>
#include <stdio.h>

#define N 5

int min(int a, int b) {
  return a < b ? a : b;
}

int master(int procs /* procs is number of slaves */) {
  int matrixA[N][N], vectorC[N];
  int i, j, dotp, row, numsent = 0;
  MPI_Status status;

  // Initialize data
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      matrixA[i][j] = i;
    }
  }

  // distribute data to slave
  for (i = 0; i < min(N, procs); i++) {
    MPI_Send(&matrixA[i][0], N, MPI_INT, i + 1, numsent, MPI_COMM_WORLD);
    numsent++;
  }

  // receive result and distribute data
  for (i = 0; i < N; i++) {
    MPI_Recv(&dotp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
      MPI_COMM_WORLD, &status);
    row = status.MPI_TAG;
    vectorC[row] = dotp;

    if (numsent < N) {
      MPI_Send(&matrixA[numsent][0], N, MPI_INT, numsent % procs + 1,
        numsent, MPI_COMM_WORLD);
      numsent++;
    }
  }

  // print the result
  for (i = 0; i < N; i++) {
    printf("%d ", vectorC[i]);
  }
  printf("\n");
  return 0;
}

int slave(int rank, int procs) {
  int vectorB[N], rowMatrixA[N];
  int i, j;

  // initialize vector B
  for (i = 0; i < N; i++) {
    vectorB[i] = 3;
  }

  for (i = 0; i < N; i++) {
    if (rank == i % procs + 1) {
      MPI_Recv(&rowMatrixA[0], N, MPI_INT, 0, i, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);

      int result = 0;
      // multily vector to matrixRow
      for (j = 0; j < N; j++) {
        result += vectorB[j] * rowMatrixA[j];
      }

      MPI_Send(&result, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
    }
  }

  return 0;
}

void main(int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)
    master(size - 1);
  else
    slave(rank, size - 1);

  MPI_Finalize();
}
