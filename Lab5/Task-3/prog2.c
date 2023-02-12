#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // gcc -o prog prog.c -lm

int main()
{
    // Check relative precision
    printf("\nFloat\n");

    float eps = 1;
    float temp;

    for (int i; i<100; i++)
    {
        if (eps+1 == 1)
        {
            printf("    1+eps = 1\n");
            printf("    eps = (0.5)^%d is too small\n", i);
            break;
        }

        temp = eps; // Store old value of eps
        eps *= 0.5; // Eps is multiplied by 0.5 for each iteration
    }

    printf("\nDouble\n");

    double eps2 = 1;
    double temp2;

    for (int i; i<100; i++)
    {
        if (eps2+1 == 1)
        {
            printf("    1+eps = 1\n");
            printf("    eps = (0.5)^%d is too small\n", i);
            break;
        }

        temp2 = eps2; // Store old value of eps
        eps2 *= 0.5; // Eps is multiplied by 0.5 for each iteration
    }








    printf("\n\n");
    return 0;
}