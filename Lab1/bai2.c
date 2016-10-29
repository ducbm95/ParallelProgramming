#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	int my_rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("Process %d in %d \n", my_rank, size);

	int snd_buf = my_rank, rcv_buf = 0, sum = 0;
	for (int i = 0; i < 10; i++) {
		int dest = my_rank + 1;
		if (dest == size)
			dest = 0;
		MPI_Send(&snd_buf, 1, MPI_INT, dest, my_rank, MPI_COMM_WORLD);

		int source = my_rank - 1;
		if (source == -1)
			source = size - 1;
		MPI_Recv(&rcv_buf, 1, MPI_INT, source, source, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		snd_buf = rcv_buf;
		printf("Process %d received value %d \n", my_rank, rcv_buf);
		sum += rcv_buf;
	}
	printf("Process %d has sum %d \n", my_rank, sum);


	MPI_Finalize();
	return 0;

}