#include <mpi.h>
#include <stdio.h>
int main( int argc, char* argv[] ) {
	int i;
	int rank, nproc;
	int isend[3], irecv;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &nproc );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	if(rank == 0) {
		for(i=0; i<nproc; i++)
			isend[i] = i;
	}
	MPI_Scatter(isend, 1, MPI_INT, &irecv, 1,MPI_INT, 0, MPI_COMM_WORLD);
	printf("irecv = %d\n", irecv);
	MPI_Finalize();
	return 0;
} 