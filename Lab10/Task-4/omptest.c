#include <stdio.h>
#include <omp.h>

// gcc -fopenmp -o omptest omptest.c

typedef struct DataForThread {
  double A;
  double B;
  double result;
  int x;
  int y;
} DataForThread_t;

void thread_func(DataForThread_t* p) {
  printf("This is the arguments inside thread_func()!\n");
  printf("    A = %f\n", p->A);
  printf("    B = %f\n", p->B);
  printf("    x = %d\n", p->x);
  printf("    y = %d\n\n", p->y);

  p->result = (p->A) * (p->B);
}

int main(int argc, char** argv) {

  const int nThreads = 2;
  DataForThread_t arr[nThreads];
  /* Prepare data for thread 0. */
  arr[0].A = 3.7;
  arr[0].B = 1.2;
  arr[0].x = 88;
  arr[0].y = 77;
  /* Prepare data for thread 1. */
  arr[1].A = 2.2;
  arr[1].B = 8.8;
  arr[1].x = 444;
  arr[1].y = 555;

#pragma omp parallel num_threads(nThreads)
  {
    /* OK, now we are inside the parallel block, so this is executed
       by several threads. Get ID of current thread. */
    int id = omp_get_thread_num();
    /* Call thread_func and give it a pointer to arr[id] as input. */
    thread_func(&arr[id]);
  }


for (int i=0; i<nThreads; i++)
{
  printf("Thread %d\n",i);
  printf("  arr[%d].res = %f\n",i,arr[i].result);
}
  

  return 0;
}
