/**
* Student Name:		Bui Minh Duc
* Student ID:		51300902
*
* Lab 2 Implementation.
* Multiply 2 vector with Collective Commutication
* @since 10/09/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main(int argc, char **argv) {
	int rank, size, i;
	int a[5] = { 1, 2, 3, 4, 5 };
	int b[5] = { 6, 7, 8, 9, 10 };
	int c[5]; // store the result
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int result = 0;
	for (i = 0; i < 5; i++) {
		if (i == rank) {
			result = a[i] * b[i];
		}
	}

	MPI_Gather(&result, 1, MPI_INT, c, 1, MPI_INT, 5, MPI_COMM_WORLD);

	if (rank == 5) {
		for (i = 0; i < 5; i++) {
			printf("%d\n", c[i]);
		}
	}

	MPI_Finalize();
}
