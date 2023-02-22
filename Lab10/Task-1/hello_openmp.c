#include <stdio.h>
#include <omp.h>

// gcc -o hello_openmp hello_openmp.c
// gcc -fopenmp -o hello_openmp hello_openmp.c

int main(int argc, char** argv) {

omp_set_num_threads(6);

#pragma omp parallel
  {
    printf("Bonjour!\n");
  }

  return 0;
}
