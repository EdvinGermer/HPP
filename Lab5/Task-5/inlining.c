#include <stdlib.h>
#include <stdio.h>

// gcc -o inlining inlining.c
int factorial (int n)            // 12.586 s
{ /* n! */
  int i, f = 1;
  for (i = 2; i <= n; i++) f *= i;
  return f;
}



// gcc -o inlining inlining.c -ffunction-sections
inline int factorial2 (int n)     // 1.887 s
{ /* n! */
  int i, f = 1;
  for (i = 2; i <= n; i++) f *= i;
  return f;
}

int main(int argc, char ** argv)
{
  int i, myFact;
  for (i=0; i<500000000; i++)
    myFact = factorial(i%12);

  printf("myFact = %d\n",myFact);
  return 0;
}

