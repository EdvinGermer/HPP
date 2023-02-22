#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

// gcc -fopenmp -o primes primes.c

// ./primes 1 21000 1

typedef struct{
    int lower,upper,temp_res;
}args_t;


// Serial code
int check_primes(int lower, int upper)
{
    int nr_of_primes = 0;
    if (lower==1)
    {
        lower = 2;
    }
    for (int num=lower;num<upper;num++){ // Check each number up to N
        int temp = 0;
        for (int i=2; i<num-1; i++)   // j is every number up to i, excluding 1 and i itself.
        {
            if (num%i==0)
            {temp += 1;}  // Not a prime
        }

        if (temp == 0)  // If i is a prime
        {nr_of_primes += 1;}
    }

    return nr_of_primes;
}



// Parallel code
void check_primes_para(args_t* ptr)
{
    int total_threads = omp_get_num_threads();
    int t = omp_get_thread_num();

    int lower = ptr->lower;
    int upper = ptr->upper;
    
    int nr_of_primes = 0;
    if (lower==1)
    {
        lower = 2;
    }
    for (int num=lower+t; num<upper ;num+=total_threads){ // Check each number up to N
        int temp = 0;
        for (int i=2; i<num-1; i++)   // j is every number up to i, excluding 1 and i itself.
        {
            if (num%i==0)
            {temp += 1;}  // Not a prime
        }

        if (temp == 0)  // If i is a prime
        {nr_of_primes += 1;}
    }

    ptr->temp_res = nr_of_primes;
}


double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}


int main(int argc, char* argv[])
{
    printf("\n\n");
    double time1, time2;
    
    if (argc < 2) {
    printf("Not enough input arguments: %s <integer>\n", argv[0]);
    return 1;
  }
    int lower = atoi(argv[1]);   // Specify lowerbound
    int upper = atoi(argv[2]);   // Specify upperbound
    int nThreads = atoi(argv[3]);   // Specify number of threads


    // Serial code for checking counting primes
    printf("Serial code:\n");
    time1 = get_wall_seconds();
    int res = check_primes(lower, upper);
    printf("        Serial code took %7.3f wall seconds.\n", get_wall_seconds()-time1);
    printf("        %d primes in the range %d->%d\n\n", res, lower, upper);




    // Parralel code for checking counting primes
    printf("Parallel code with %d threads:\n",nThreads);

    // Prepare arguments
    args_t ptr[nThreads]; 
    for(int t=0;t<nThreads;t++)
    {
        ptr[t].lower = lower;
        ptr[t].upper = upper;
    }

    // Calculations
    double start = omp_get_wtime();
    #pragma omp parallel num_threads(nThreads)
    {
    int t = omp_get_thread_num();
    check_primes_para(&ptr[t]);
    }

    // Combine results
    int result = 0;
    for(int t=0;t<nThreads;t++)
        result += ptr[t].temp_res;

    printf("        Parallel code took %7.3f wall seconds.\n", omp_get_wtime()-start);
    printf("        %d primes in the range %d->%d\n\n", result, lower, upper);
    
   

    printf("\n\n");
    return 0;
}