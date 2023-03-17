#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

// gcc -o serial_lu serial_lu.c -lm

// gcc -ffast-math -march=native -o serial_lu serial_lu.c -lm
// gcc -O1 -o serial_lu serial_lu.c -lm
// gcc -O2 -o serial_lu serial_lu.c -lm
// gcc -O3 -o serial_lu serial_lu.c -lm
// gcc -Ofast -o serial_lu serial_lu.c -lm

// gcc -ffast-math -march=native -Ofast -o serial_lu serial_lu.c -lm


void print_mat(double** m, int n)
{
    for (int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
                printf("   %f   ", m[i][j]);
            printf("\n");
        }
    printf("\n");
}

static double get_wall_seconds()
{ // FUNCTION TAKEN FROM THE LABSs
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}


void check(double** A,double** L,double** U,int n)
{
    // Allocate memory
    double** temp2 = (double **)malloc(n*sizeof(double *)); 
    for(int i=0;i<n;i++)
        temp2[i] = (double *)malloc(n*sizeof(double));

    // Reconstruct A from L and U
    double res;
    for (int row=0;row<n;row++)
        for (int col=0;col<n;col++)
            {
                res = 0;
                for (int i=0;i<n;i++)
                    res += L[row][i]*U[i][col];
                temp2[row][col] = res;
            }
        
    // Check error
    double error = 0.0;
    for (int row=0;row<n;row++)
        for (int col=0;col<n;col++)
            error += abs(A[row][col] - temp2[row][col]);

    printf("Total error = %0.15f\n\n",error);
    
    // Print results
    if (error>0.0000000001)
        printf("LU Factorization NOT succesful!\n");

    else
        printf("LU Factorization OK!\n");


    // Free memory
    for(int i=0;i<n;i++)
        free(temp2[i]);
    free(temp2);
}




void lu_fac(double** restrict L, double** restrict U, const int n)
{
    // Initialize
    int i;
    
    // LU factorize
    for (int col=0;col<n;col++)
    {
        double temp1 = 1/U[col][col];
        for (int row=col+1;row<n;row++)
        {
            double temp2 = U[row][col]*temp1;
            L[row][col] = temp2;

            for (i=0;i<n-4;i+=4)  // Loop unrolling
                {
                    U[row][i] -=  U[col][i]*temp2;
                    U[row][i+1] -=  U[col][i+1]*temp2;
                    U[row][i+2] -=  U[col][i+2]*temp2;
                    U[row][i+3] -=  U[col][i+3]*temp2;
                }
            
            for (i;i<n;i++) // If leftover
                U[row][i] -=  U[col][i]*temp2;
        }
    }
}


int main(int argc, char *argv[])
{
    /*############### Read arguments ##############*/
    if (argc != 2)
    {printf("Error: This function takes 1 arguments, but got %d\n", argc - 1);}
    
    const int n = atoi(argv[1]);     // Size of matrix


    /*############### Initialize variables ##############*/
    int i,j;
    double** A; // Matrix
    double** L; // Matrix
    double** U; // Matrix


    // Allocate memory
    A = (double **)malloc(n*sizeof(double *)); // A matrix
    for(i=0;i<n;i++)
        A[i] = (double *)malloc(n*sizeof(double));

    L = (double **)malloc(n*sizeof(double *)); // L matrix
    for(i=0;i<n;i++)
        L[i] = (double *)malloc(n*sizeof(double));

    U = (double **)malloc(n*sizeof(double *)); // U matrix
    for(i=0;i<n;i++)
        U[i] = (double *)malloc(n*sizeof(double));
    

 // Fill matrices
    srand(time(0)); // Random seed
    for (i = 0; i<n; i++)
        for(j=0;j<n;j++)
            {
                A[i][j] = rand() % 9 + 1;   // A is filled with random numbers
                U[i][j] = A[i][j];          // U is a copy of A initially

                if (i==j)
                {L[i][j] = 1;}              // L is a unit matrix initially
                else
                {L[i][j] = 0;}
            }


    // LU Factorization
    double start = get_wall_seconds();
    lu_fac(L,U,n);
    printf("    Completed in %f seconds\n",get_wall_seconds() - start);

    // Check if correct
    printf("\nChecking if correct...\n");
    check(A,L,U,n);

    
    // Free memory
    for(i=0;i<n;i++)
        free(A[i]);
    free(A);

    for(i=0;i<n;i++)
        free(L[i]);
    free(L);

    for(i=0;i<n;i++)
        free(U[i]);
    free(U);

    return 0;
}