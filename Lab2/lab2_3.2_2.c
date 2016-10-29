/**
* Student Name:   Bui Minh Duc
* Student ID:     51300902
*
* Lab 2 Implementation.
* Multiply 2 Matrix using Collective Communicator.
* @since 18/09/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define M_SIZE 5 // matrix size

int a[M_SIZE][M_SIZE];
int b[M_SIZE][M_SIZE];
// int c[M_SIZE][M_SIZE]; // store result of multiplication.
int c[M_SIZE * M_SIZE];

// initialize value for matrix
void initMatrix() {
  int i, j;
  for (i = 0; i < M_SIZE; i++) {
    for (j = 0; j < M_SIZE; j++) {
      a[i][j] = i + j;
      b[i][j] = j - i;
    }
  }
}

void main(int argc, char **argv) {
  initMatrix();
  int rank, size, i, j, k;
  double start, end;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  start = MPI_Wtime();

  if (rank > 0) {
    // config for non-root processors
    for (i = 0; i < M_SIZE; i++) {
      for (j = 0; j < M_SIZE; j++) {
        int currentPoint = i * M_SIZE + j;
        int calRank = currentPoint % (size - 1);
        if (rank == calRank + 1) {
          c[currentPoint] = 0; // store result of  element value
          for (k = 0; k < M_SIZE; k++) {
            c[currentPoint] += a[i][k] * b[k][j];
          }
        }
      }
    }
  }

  for (i = 0; i < M_SIZE * M_SIZE; i++) {
    int src = i % (size - 1) + 1;
    MPI_Bcast(&*(c + i), 1,  MPI_INT, src, MPI_COMM_WORLD);
  }

  // print result
  // if (rank == 0) {
  //   for (i = 0; i < M_SIZE; i++) {
  //    for (j = 0; j < M_SIZE; j++) {
  //      printf("%d ", c[i * M_SIZE + j]);
  //    }
  //    printf("\n");
  //   }
  // }
  if (rank == 0) {
    end = MPI_Wtime();
    printf("Time: %f\n", end - start);
  }

  MPI_Finalize();
}
