#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

// gcc -o serial_lu serial_lu_pivot.c -lm


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
    double** temp = (double **)malloc(n*sizeof(double *)); 
    for(int i=0;i<n;i++)
        temp[i] = (double *)malloc(n*sizeof(double));

    // Reconstruct A from L and U
    double res;
    for (int row=0;row<n;row++)
        for (int col=0;col<n;col++)
            {
                res = 0;
                for (int i=0;i<n;i++)
                {
                    res += L[row][i]*U[i][col];
                }
                temp[row][col] = res;
            }
        
    // Check error
    double error = 0.0;
    for (int row=0;row<n;row++)
        for (int col=0;col<n;col++)
            error += abs(A[row][col] - temp[row][col]);

    printf("Total error = %0.15f\n\n",error);
    
    // Print results
    if (error>0.0000000001)
    {
        printf("LU Factorization NOT succesful!\n");
    }
    else
    {
        printf("LU Factorization OK!\n");
    }

    // Free memory
    for(int i=0;i<n;i++)
        {
            free(temp[i]);
        }
    free(temp);

}




void lu_fac(double** L, double** U, int n)
{
    // LU factorize
    
    double max,temp;
    int pivot_idx;

    for (int col=0;col<n-1;col++)
    {
        max = U[col][col]; // Default pivot is diagonal
        pivot_idx = col;   // Default pivot row is current diagonal
        
        for(int row=col+1;row<n;row++) // find largest pivot in column
            {
                if( U[row][col] > max )
                {
                    max = U[row][col];
                    pivot_idx = row;
                }
            }
        
        if (pivot_idx != col) // If a larger pivot is found
        {
            // Swap rows in U matrix
            for(int j=col;j<n;j++) 
                {
                    temp = U[col][j];
                    U[col][j] = U[pivot_idx][j]; // Pivot row to top
                    U[pivot_idx][j] = temp;      // Old top moves down
                }

            // Swap rows in L matrix
            for(int j=0;j<col;j++) 
                {
                    temp = L[col][j];
                    L[col][j] = L[pivot_idx][j]; // Pivot row to top
                    L[pivot_idx][j] = temp;      // Old top moves down
                }
        }
        for (int row=col+1;row<n;row++)
        {
            L[row][col] = U[row][col]/max;
            
            for (int i=col;i<n;i++)
                U[row][i] = U[row][i] - U[col][i]* L[row][col];
        }
    }
}


int main(int argc, char *argv[])
{
    
    printf("\n");
    /*############### Read arguments ##############*/
    if (argc != 2)
    {printf("Error: This function takes 1 arguments, but got %d\n", argc - 1);}
    
    int n = atoi(argv[1]);     // Size of matrix


    /*############### Initialize variables ##############*/
    int i,j,k;
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


    // Prints
   // printf("Print matrix A: \n");
    //print_mat(A,n); 

    // LU Factorization
    printf("\nLU Factoriztion...\n");
    double start = get_wall_seconds();
    lu_fac(L,U,n);
    printf("    Completed in %f seconds\n",get_wall_seconds() - start);

    // Prints
   // printf("\n\nPrint matrix L: \n");
    //print_mat(L,n);

    // Prints
    //printf("Print matrix U: \n");
    //print_mat(U,n);

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


    printf("\n\n");
    return 0;
}