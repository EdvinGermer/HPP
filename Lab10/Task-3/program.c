#include <stdio.h>
#include <omp.h>

// gcc -fopenmp -o program program.c

void thread_func() {
  int available = omp_get_num_threads();
  int current = omp_get_thread_num();

  printf("  Thread %d out of %d\n",current,available-1);

}

int main(int argc, char** argv) {

#pragma omp parallel num_threads(4)
  {
    thread_func();
  }

  return 0;
}
