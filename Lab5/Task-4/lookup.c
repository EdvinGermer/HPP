#include <stdlib.h>
#include <stdio.h>

/* From A. Fog: Example 14.1a. */


/* WHAT DOES THE FUNCTION DO
      Calculate the factorial for 50.000.000 numbers
      If the same values are computed many times during program execution, saving
      precomputed values in a table is often a good idea.

      If FAST = 0:
        Use regular math
         
      
      If FAST = 1:
        Use lookup table
         
*/




#define FAST 0

#if FAST
int factorial (int n)
{  /* n! */
   /* Table of factorials: */
  static const int FactorialTable[13] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
  if ((unsigned int)n < 13) { /* Bounds checking (see page 137). */
    return FactorialTable[n]; /* Table lookup. */
  }
  else {
    return 0; /* return 0 if out of range. (a better solution may be to call some other function in this case, to get correct result) */
  }
}

#else
int factorial (int n) 
{ /* n! */
  int i, f = 1;
  for (i = 2; i <= n; i++) f *= i;
  return f;
}
#endif

int main(int argc, char ** argv)
{
  int i, myFact;
  for (i=0; i<50000000; i++)
    myFact = factorial(i%12);

  printf("myFact = %d\n",myFact);
  return 0;
}

