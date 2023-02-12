#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    int num, copy, remainder, reverse;
    printf("Input: "); scanf("%d", &num);

    // Initialize numbers
    remainder=1; 
    copy = num,
    reverse = 0;
    while(copy>0)
    {  
        remainder = copy%10;
        reverse = 10*reverse + remainder;
        

        copy = copy/10;
    }

    printf("\nYour number is: %d", num);
    printf("\nReverse is: %d\n", reverse);

    if (reverse == num)
    {
        printf("it is a palindrome\n");
    }
    else
    {
        printf("it is not a palindrome\n");
    }

}