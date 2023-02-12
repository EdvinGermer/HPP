#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double d = 1;
    int i = 1;
    char c = 'x';

    printf("Double: %f, at addres: %p, with size: %ld bytes \n", d, &d, sizeof(d));
    printf("Int:    %d,        at addres: %p, with size: %ld bytes \n", i, &i,sizeof(i));
    printf("Char    %d,      at addres: %p, with size: %ld bytes \n", c, &c,sizeof(c));
}