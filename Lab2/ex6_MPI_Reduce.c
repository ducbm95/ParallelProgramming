#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int i, rank, size;
	int sendBuf, recvBuf, count;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	sendBuf = rank;
	count = size;

	// cong tat ca cac gia tri cua bien sendBuf vao
	// bien recvBuf
	MPI_Reduce(&sendBuf, &recvBuf, 1, MPI_INT, 
		MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Sum is %d\n", recvBuf);
	}

	MPI_Finalize();
	return 0;
}
