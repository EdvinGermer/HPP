#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int factorio(int num)  // Recursive function for calculating the factorial of a number
{
    int res;
    
    if (num==0)
    {
        res = 1;
    }
    else
    {
        res = num * factorio(num-1);
    } 

    return res;
}




int main(int argc, char *argv[] )   // Main function that takes in the number of rows
{
    int n = atoi(argv[1]);  // n = nr of rows

    for (int i=0; i<n; i++) // Iterate over rows
    {   
        for (int j=0; j<=i; j++)  // Iterate over columns
        {
            int el = factorio(i) / ((factorio(j)*factorio(i-j))); // calculate matrix element
            printf("%d    ", el);
        }
        printf("\n");
    }
    
    return 0;
}