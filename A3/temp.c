#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>

// gcc -o temp temp.c -lm

int main()
{
    double e0 = 10e-3;
    
    double x_i = 0.519101;
    double y_i = 0.298794;

    double x_j = 0.569305;
    double y_j = 0.248582;
    
    double r_x = x_i - x_j;
    double r_y = y_i - y_j;

    double r = sqrt( pow(r_x,2) + pow(r_y,2) );
    double r3 = pow(r + e0,3);
    double r3_2 = (r + e0)*(r + e0)*(r + e0);


    printf("\nTEST\n");
    printf("x_i = %f\n", x_i);
    printf("y_i = %f\n\n", y_i);

    printf("x_j = %f\n", x_j);
    printf("y_j = %f\n\n", y_j);

    printf("r_x = %f\n", r_x);
    printf("r_y = %f\n\n", r_y);

    printf("r = %f\n", r);
    printf("r3 = %f\n", r3);
    printf("r3_2 = %f\n", r3_2);






    return 0;
}