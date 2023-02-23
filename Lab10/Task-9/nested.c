#include <stdio.h>
#include <omp.h>

// gcc -fopenmp -o nested nested.c

void thread_func() {
  int available = omp_get_num_threads();
  int current = omp_get_thread_num();

  printf("  Thread %d out of %d\n",current,available-1);

}

int main(int argc, char** argv) {

    int nThreads = 8;

    // Activate nested parallelism
    omp_set_nested(1);

    if (omp_get_nested())
        printf("Nested parallelism is enabled\n");
    else
        printf("Nested parallelism is disabled\n");

    #pragma omp parallel num_threads(nThreads)
    {
        if (omp_get_nested())
            {
                #pragma omp parallel num_threads(nThreads)
                {
                    thread_func();
                }
            }
        else
            thread_func();
    }

  return 0;
}
