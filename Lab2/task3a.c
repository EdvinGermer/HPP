#include <stdio.h>

int main(void)
{
    int x, y;
    
    printf("Give 2 integers: ");
    scanf("%d %d", &x, &y);

    if (x%2==0 && y%2==0)
    {
        printf("\nBoth are even, sum is: %d \n", x+y);
    }

    else
    {
        printf("\nBoth are not even, product is: %d \n", x*y);
    }
}