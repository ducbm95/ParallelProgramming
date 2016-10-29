/**
* Student Name:		Bui Minh Duc
* Student ID:			51300902
*
* Lab 2 Implementation.
* Multiply 2 Matrix using MPI_Send, MPI_Recv.
* @since 12/09/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define M_SIZE 1000 // matrix size

int a[M_SIZE][M_SIZE];
int b[M_SIZE][M_SIZE];
int c[M_SIZE][M_SIZE]; // store result of multiplication.

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
	int rank, size, i, j, k, tag;
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
					int x = 0; // store result of  element value
					for (k = 0; k < M_SIZE; k++) {
						x += a[i][k] * b[k][j];
					}
					tag = currentPoint / (size - 1);
					MPI_Send(&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
				}
			}
		}
	}
	else {
		// config for root processor
		int mtxCount = M_SIZE * M_SIZE; // number of element in matrix
		for (i = 0; i < mtxCount; i++) {
			int row = i / M_SIZE;
			int col = i % M_SIZE;
			int src = i % (size - 1) + 1;
			tag = i / (size - 1);
			int result;
			MPI_Recv(&result, 1, MPI_INT, src, tag,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			c[row][col] = result;
		}

		// print result
		// for (i = 0; i < M_SIZE; i++) {
		// 	for (j = 0; j < M_SIZE; j++) {
		// 		printf("%d ", c[i][j]);
		// 	}
		// 	printf("\n");
		// }

		end = MPI_Wtime();
		printf("Time: %f\n", end - start);
	}

	MPI_Finalize();
}
