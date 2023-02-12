#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

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
    double* args = (double*) (data);

    double l_bound = args[0];
    double u_bound = args[1];

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

    args[2] = nr_of_primes;
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


    // Serial code for checking counting primes
    printf("Serial code:\n");
    time1 = get_wall_seconds();
    int res = check_primes(lower, upper);
    printf("        Serial code took %7.3f wall seconds.\n", get_wall_seconds()-time1);
    printf("        %d primes in the range %d>%d\n\n", res, (int)(lower), (int)(upper));




    // Parralell code for checking counting primes
    printf("Parallell code:\n");
    pthread_t thread;

    double fac = 0.7;  // Fraction of how much should be on main

    double args[3];
    args[0] = upper;
    args[1] = upper;
    args[2] = res;

    time2 = get_wall_seconds();
    pthread_create(&thread, NULL, check_primes_thread, args); // Second fraction on thread
    int res1 = check_primes(lower, upper*fac); // First fraction on main
    pthread_join(thread, NULL);   

    printf("        Parallell code took %7.3f wall seconds.\n", get_wall_seconds()-time2);
    printf("        %d primes in the range %d->%d\n", (int)(res1+args[2]), (int)(lower), (int)(upper));


    printf("\n\n");
    return 0;
}