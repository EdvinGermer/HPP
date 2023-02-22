#include <stdio.h>
#include <omp.h>


// gcc -fopenmp -o Ntest Ntest.c

const long int NA = 200000000;
const long int NB = 600000000;

long int thread_func_A() {
  long int i;
  long int sum = 0;
  for(i = 0; i < 10*NA; i++)
    sum += 7;
  return sum;
}

long int thread_func_B() {
  long int i;
  long int sum = 0;
  for(i = 0; i < 10*NB; i++)
    sum += 7;
  return sum;
}

int main() {
  printf("This is the main() function starting.\n");

  long int result_A;
  long int result_B;

double start = omp_get_wtime();
#pragma omp parallel num_threads(2)
  {
    int id = omp_get_thread_num();
    if(id == 0)
      result_A = thread_func_A();
    else
      result_B = thread_func_B();
  }

  printf("This is the main() function after the parallel block.\n");

  printf("result_A : %ld\n", result_A);
  printf("result_B : %ld\n", result_B);
  long int totalSum = result_A + result_B;
  printf("totalSum : %ld\n", totalSum);


  printf("Wall time is %fs\n", omp_get_wtime()-start);

  return 0;
}
