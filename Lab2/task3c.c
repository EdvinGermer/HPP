#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a,b,c;
    scanf("%d %d %d", &a, &b, &c);

    if ((abs(a) > abs(b) && abs(a) < abs(c)) || (abs(a) > abs(c) && abs(a) < abs(b)))
    {
        printf("The first number (%d) is the second largest in magnitude", a);
    }

    else if ((abs(b) > abs(a) && abs(b) < abs(c)) || (abs(b) > abs(c) && abs(a) < abs(a)))
    {
        printf("The second number (%d) is the second largest in magnitude", b);
    }

    else if ((abs(c) > abs(b) && abs(c) < abs(a)) || (abs(c) > abs(a) && abs(c) < abs(b)))
    {
        printf("The last number (%d) is the second largest in magnitude", c);
    }

    printf("\n");


}