#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    int num, den;
    printf("Enter dividend: "); scanf("%d", &num);
    printf("Enter divisor: "); scanf("%d", &den);
    
   int quot = num/den;
   int remainder = num%den;

    printf("Quotient: %d\n", quot);
    printf("Remainder: %d\n", remainder);
}