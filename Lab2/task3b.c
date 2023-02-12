#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a,b,c;
    scanf("%d %d %d", &a, &b, &c);

    if (abs(a) > abs(b) && abs(a) > abs(c))
    {
        printf("The first number (%d) is the largest in magnitude", a);
    }

    if (abs(b) > abs(a) && abs(b) > abs(c))
    {
        printf("The second number (%d) is the largest in magnitude", b);
    }

    if (abs(c) > abs(a) && abs(c) > abs(b))
    {
        printf("The last number (%d) is the largest in magnitude", c);
    }

    printf("\n");


}