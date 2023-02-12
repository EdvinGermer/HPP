#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    int x;
    printf("Enter a number: "); scanf("%d", &x);
    
    int sqr = sqrt(x);
    printf("\nx = %d", x);
    printf("\nsqr = %d", sqr);
    printf("\nsqr^2 = %d\n", sqr*sqr);

    if (sqr*sqr == x)
    {
        printf("\nYour number is a perfect square\n");
    }
    else
    {
        printf("\nYour number is not a perfect square\n\n");
    }

}