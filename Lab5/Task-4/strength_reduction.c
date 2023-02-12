#include <stdio.h>
#include <stdlib.h>


/* WHAT DOES THE FUNCTION DO
Compute a lot of math expressions in a for loop 50.000.000 times


*/


int main(int argc, char **argv)
{
   int i;
   int a = 1, b = 2, c = 3, d = 1;
   float x = 0.1, y = 0.5, z = 0.33;
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);

   for (i=0; i<50000000; i++)
   {
      /*
      c=d*2;
      b=c*15;
      a=b/16;
      d=b/a;

      z=0.33;
      y=2*z;
      x=y/1.33;
      z=x/1.33;
      */
      
      c = d<<1;   // d*2     
      b=(c<<4)-c; // c*15;
      a = b>>4;   // b/16;
      d=b/a;

      z=0.33;
      y=2*z;
      x=y*0.7518796992; //   y/1.33;
      z=x*0.7518796992; //   x/1.33;
   }
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);
   return 0;
}
