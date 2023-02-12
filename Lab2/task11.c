#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i=0;
    int *arr;

    arr = (int *)malloc(10*sizeof(int)); // Start with memory allocate 10 integers
    
    printf("Input: ");
    while (scanf("%d", &arr[i]) != 0)  // Read numbers and store in array until you reach non-integer
    {
        i += 1;

        if (i%10==0)
        {
            arr = realloc(arr,(10+i)*sizeof(int)); // increase memory allocation by 10
        }
    }


    // Print the array
    int sum=0;
    printf("Output:\n");
    for (int idx=0; idx<i; ++idx)
    {
        sum += arr[idx];
        printf("%d ", arr[idx]);
    }
    printf("\n");

    printf("Sum: %d\n", sum);
}