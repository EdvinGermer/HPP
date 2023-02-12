#include <stdio.h>
#include <stdlib.h>

// Poiners point to the same adresses but update the values at those addresses.
// After function is executed the pointers a and b will still point to the same memory address
void swap_nums(int* a, int* b)
{
int temp = *a;

*a = *b;       
*b = temp;     
}

// Keep the value at each address constant but change the pointers address
// After function is executed the pointers a and b will point to new memory addresses
void swap_pointers(char** s1, char** s2) 
{
    char* temp = *s1;  

    *s1 = *s2;         
    *s2 = temp;       
}



int main()
{
    int a,b;
    char *s1,*s2;

    a = 3; b=4;

    swap_nums(&a,&b);
    printf("a=%d, b=%d\n", a, b);

    s1 = "second"; s2 = "first";
    swap_pointers(&s1,&s2);
    printf("s1=%s, s2=%s\n", s1, s2);
    return 0;
}