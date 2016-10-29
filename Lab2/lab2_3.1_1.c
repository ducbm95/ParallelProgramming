/**
* Student Name:		Bui Minh Duc
* Student ID:		51300902
*
* Lab 2 Implementation.
* Multiply 2 vector with MPI_Send, MPI_Recv.
* This example multiply 2 vector with size equal to 5.
* Need to run with at least 6 proccessors.
* @since 10/09/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DEST 5

void main(int argc, char **argv) {
	int rank, size, i;
	int a[5] = { 1, 2, 3, 4, 5 };
	int b[5] = { 6, 7, 8, 9, 10 };
	int c[5]; // store result of multiplication.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// each of processors multiply element of vector and send
	// result to processor 5.
	for (i = 0; i < 5; i++) {
		if (rank == i) {
			int mul = a[i] *  b[i];
			MPI_Send(&mul, 1, MPI_INT, DEST, rank, 
				MPI_COMM_WORLD);
		}
	}

	// processor 5 receive result from other processors 0-4
	// and assign result to vector c
	if (rank == DEST) {
		for (i = 0; i < 5; i++) {
			int result;
			MPI_Recv(&result, 1, MPI_INT, i, MPI_ANY_TAG, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			c[i] = result;
		}
	}

	MPI_Finalize();
}