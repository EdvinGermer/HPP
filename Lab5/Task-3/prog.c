#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // gcc -o prog prog.c -lm

int main()
{
    // Float
    printf("\nFloat\n");
    float x = 5;
    printf("    x_0 = %lf\n", x);
    printf("        .\n");
    printf("        .\n");
    printf("        .\n");
    
    for (int i = 1; i<25; i++)
    {
        float temp = x; //copy of x
        x = 100*x;
        
        if (isinf(x))
        {
            printf("    x_%d = 5 * 100^%d = %lf\n", i-1, i-1, temp);
            printf("    x_%d = 5 * 100^%d = %lf\n", i, i, x);
            
            printf("    x_%d + 100 = %lf\n", i,x+100);
            break;
        }
    }


    // Double
    printf("\nDouble\n");
    double y = 5;
    printf("    x_0 = %lf\n", y);
    printf("        .\n");
    printf("        .\n");
    printf("        .\n");
    
    for (int i = 1; i<1000; i++)
    {
        double temp = y; //copy of x
        y = 100*y;
        
        if (isinf(y))
        {
            printf("    x_%d = 5 * 100^%d = %lf\n", i-1, i-1, temp);
            printf("    x_%d = 5 * 100^%d = %lf\n", i, i, y);
            
            printf("    x_%d + 100 = %lf\n", i,y+100);
            break;
        }
    }


    // NaN
    printf("\nNaN\n");

    int z = -100;

    printf("    z = %d\n",z);
    printf("    sqrt(z) = %f\n", sqrt(z));
    printf("    sqrt(z)+100 = %f\n", sqrt(z)+100);

    printf("\n\n");
    return 0;
}