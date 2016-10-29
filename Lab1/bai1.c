#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_LOOP 10
#define TAG_P0 100
#define TAG_P1 101

int main(int argc, char **argv) {
	// char *s1 = "ping", *s2 = "pong"; // s1 is start address of s1 pointer
	// char *temp = (char *) malloc(4); // intialize data in memory for temp

	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double t1 = 0.0, t2 = 0.0;
	for (int i = 0; i < NUM_LOOP; i++) {
		if (rank == 0) {
			t1 = MPI_Wtime();
			MPI_Send(&t1, 1, MPI_DOUBLE, 1, TAG_P0, MPI_COMM_WORLD);
			MPI_Recv(&t1, 1, MPI_DOUBLE, 1, TAG_P1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			t2 = MPI_Wtime();
			printf("Process0 received data, time is %f \n", t2 - t1);
		} else {
			MPI_Recv(&t1, 1, MPI_DOUBLE, 0, TAG_P0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			t2 = MPI_Wtime();
			printf("Process1 received data, time is %f \n", t2 - t1);
			t1 = MPI_Wtime();
			MPI_Send(&t1, 1, MPI_DOUBLE, 0, TAG_P1, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;

}