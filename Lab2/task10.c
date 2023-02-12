#include <stdio.h>
#include <stdlib.h>


void print_array(int *arr,int n)
{
    for (int i=0; i<n; ++i)
    {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}


int main()
{
    // Initialize int-array and variable n
    int *arr;
    int n;
   
    // Take input for size of array
    scanf("%d", &n);

    // Allocate memory for the array
    arr = (int *)malloc(n*sizeof(int));
    
    // Fill the array with numbers from 0 to 99
    for(int i=0; i<n; ++i) arr[i] = rand() % 100; // random number from 0 to 99
    
    // Print all values of the integer array
    print_array(arr, n);
    
    return 0;
}