#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int a, b;
  a = 100;
  b = 20;

#pragma omp parallel firstprivate(a)    // Changed from private(a) to firstprivate(a)
  {
    a += b;
    printf("a= %d b= %d\n",a,b);
  }


  return 0;
}
