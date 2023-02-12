#include <stdio.h>

int main(void)
{
    int a, b, i, j;
    
    printf("Provide the number a and b (a x b): ");
    scanf("%d%d", &a, &b);

    printf("\n");
    // First row
    for (i=0; i<b; i++)
    {
        printf("*");
    }

    // Middle part
    for (i=0; i<a-2; i++)
    {
        printf("\n*");
        for (j=0; j<b-2; j++)
        {
            printf(".");
        }
        printf("*");
    }

    // Bottom row
    printf("\n");
    for (i=0; i<b; i++)
    {
        printf("*");
    }
    printf("\n\n");
}