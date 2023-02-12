#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double num1, num2, res;
    char op;

    printf("Input expression: ");
    scanf("%lf %c %lf", &num1, &op, &num2);

    switch(op)
    {
        case '+':
        res = num1+num2;
        break;

        case '-':
        res = num1-num2;
        break;

        case '*':
        res = num1*num2;
        break;

        case '/':
        if (num2 != 0)
        {
            res = num1/num2;
        }
        else
        {
            printf("Division by zero not allowed\n");
            exit(0);
        }
        
        break;

        default:
        printf("Invalid  input");
    }
    
    printf("Result: %lf \n", res);

}