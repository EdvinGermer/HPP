#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>


// gcc -o n_threads_primes n_threads_primes.c -lpthread
// ./n_threads_primes 1 10 4     ->   find all primes from 1 to 10 with 4 threads


struct data{
    int lower;
    int upper;
    int res;
};


int check_primes(double l_bound, double u_bound)
{
    int nr_of_primes = 0;
    if (l_bound==1)
    {
        l_bound = 2;
    }
    for (int num=l_bound;num<u_bound;num++){ // Check each number up to N
        int temp = 0;
        for (int i=2; i<num-1; i++)   // j is every number up to i, excluding 1 and i itself.
        {
            if (num%i==0)
            {temp += 1;}  // Not a prime
        }

        if (temp == 0)  // If i is a prime
        {nr_of_primes += 1;}
        //printf("%d, ",num);}
    }

    return nr_of_primes;
}


void* check_primes_thread(void* data)
{
    struct data* args = (struct data*) (data);

    double l_bound = args->lower;
    double u_bound = args->upper;;

    if (l_bound==1)
    {
        l_bound = 2;
    }
    
    int nr_of_primes = 0;
    for (int num=l_bound;num<u_bound;num++){ // Check each number up to N
        int temp = 0;
        for (int i=2; i<num-1; i++)   // j is every number up to i, excluding 1 and i itself.
        {
            if (num%i==0)
            {temp += 1;}  // Not a prime
        }

        if (temp == 0)  // If i is a prime
        {nr_of_primes += 1;}
        //printf("%d, ",num);}
    }

    args->res = nr_of_primes;
    return NULL;
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
    double lower = atoi(argv[1]);   // Specify lowerbound
    double upper = atoi(argv[2]);   // Specify upperbound
    double N = atoi(argv[3]);       // Specify nr of threads


    pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));
    struct data* d = (struct data*) malloc(N * sizeof(struct data));


    // Serial code for checking counting primes
    printf("Serial code:\n");
    time1 = get_wall_seconds();
    int res = check_primes(lower, upper);
    printf("        Serial code took %7.3f wall seconds.\n", get_wall_seconds()-time1);
    printf("        %d primes in the range %d>%d\n\n", res, (int)(lower), (int)(upper));




    // Parralell code for checking counting primes
    printf("Parallell code:\n");
    
    time2 = get_wall_seconds();
    for (int i=0; i<N; i++)
    {
       // d is an array of structs
       d[i].lower = lower + (upper - lower) * i / N;
       d[i].upper = lower + (upper - lower) * (i + 1) / N;

        pthread_create(&threads[i], NULL, check_primes_thread, &d[i]);
    }

   
    // Join all threads
    for (int i; i<N; i++)
    {pthread_join(threads[i], NULL);}


    // Sum up results from all threads
     int tot = 0;
    for (int i=0; i<N; i++)
    {tot += (int) (d[i].res);}

    // Print results
    printf("        Parallell code took %7.3f wall seconds.\n", get_wall_seconds()-time2);
    printf("        %d primes in the range %d->%d\n", tot, (int)(lower), (int)(upper));

    // Free memory
    free(d);
    free(threads);

    printf("\n\n");
    return 0;
}