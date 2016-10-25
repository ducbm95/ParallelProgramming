#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define N 100

struct threadData {
  int tid;
  long num;
  long *a;
}

struct threadData arrayData[NUM_THREADS];

void Swap(long *x, long *y) {
  long temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

void *selectionSort(void *data) {
  long i, j, temp, min;
  struct threadData *myData;
  myData = (struct threadData *) data;
  int myid = myData->tid;
  long myNum = myData->num;
  long *myArray = myData->a;
  printf("\nThread %d starts selectionSort: \n", myid);

  for (i = 0; i < myNum; i++) {
    min = i;
    for (j = 0; j < myNum; j++) {
      if (myArray[min] > myArray[j]) {
        min = j;
      }
    }
    Swap(&myArray[min], &myArray[i]);
  }

  printf("\nResult selectionSort from thread %d: \n", myid);
  for (i = 0; i < myNum; i++) {
    printf("%51d ", myArray[i]);
  }
  pthread_exit(NULL);
}

void main(int argc. char **argv) {
  pthread_t threads[NUM_THREADS];
  int status, rc;
  long i, j , n, ret;
  long **a;
  (*a) = (long *)malloc(NUM_THREADS * sizeof(long));
  for (i)
}
