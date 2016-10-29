#include <mpi.h>
#include <stdio.h>

#define N 1000

int a[N][N];
int b[N][N];
int c[N][N];
int matrix_size = N * N;

void master(int nWorkers) {
  int i, j, x;
  MPI_Status status;
  // generate matrix a and b
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i;
      b[i][j] = i;
    }
  }

  // send all matrix data
  for (i = 1; i <= nWorkers; i++) {
    MPI_Send(&a, matrix_size, MPI_INT, i, 0, MPI_COMM_WORLD);
    MPI_Send(&b, matrix_size, MPI_INT, i, 1, MPI_COMM_WORLD);
  }

  // receive data from worker
  for (i = 0; i < matrix_size; i++) {
    MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    int pos = status.MPI_TAG;
    int src = status.MPI_SOURCE;
    c[pos / N][pos % N] = x;

    if (i == matrix_size - 1) {
      // send terminal message to all worker
      for (j = 1; j <= nWorkers; j++) {
        MPI_Send(&i, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
      }
    } else if (i >= nWorkers - 1) {
      // send a signal to worker to calculate next value in matrix
      MPI_Send(&i, 1, MPI_INT, src, 0, MPI_COMM_WORLD);
    }
  }

  // print the result
  // for (i = 0; i < N; i++) {
  //   for (j = 0; j < N; j++) {
  //     printf("%d ", c[i][j]);
  //   }
  //   printf("\n");
  // }
}

void worker(int rank) {
  int i, x, row, col, pos;
  int firstTime = 1;
  // receive matrix data
  MPI_Recv(&a, matrix_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv(&b, matrix_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  while (1) {
    if (firstTime) {
      pos = rank - 1;
      firstTime = 0;
    } else {
      MPI_Recv(&pos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (pos == matrix_size - 1)
        break;
      pos += 1;
    }
    x = 0;
    row = pos / N;
    col = pos % N;
    for (i = 0; i < N; i++) {
      x += a[row][i] * b[i][col];
    }
    MPI_Send(&x, 1, MPI_INT, 0, pos, MPI_COMM_WORLD);
  }

  printf("P%d termiated\n", rank);
}

void main(int argc, char **argv) {
	int rank, size;
	MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    master(size - 1);
  } else {
    worker(rank);
  }

  MPI_Finalize();
}
