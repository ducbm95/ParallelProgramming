#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char **argv) {
  int *a;
  MPI_Win win;
  MPI_Init(&argc, &argv);

  MPI_Alloc_mem(1000 * sizeof(int), MPI_INFO_NULL, &a);
  a[0] = 1;
  a[1] = 2;

  MPI_Win_create(a, 1000 * sizeof(int), sizeof(int), MPI_INFO_NULL,
    MPI_COMM_WORLD, &win);

  MPI_Win_free(&win);
  MPI_Free_mem(a);
  MPI_Finalize();
}
