#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>


// gcc -o n_threads n_threads.c -lpthread

struct data
{
    int index;
};


void* thread_func(void* args)
{
    struct data* data = (struct data*) (args);

    printf("    Thread %d\n", data->index);
}


double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}


int main(int argc, char* argv[])
{
    printf("\n\nStarting main function\n");
    
    if (argc < 2) {
    printf("Not enough input arguments: %s <integer>\n", argv[0]);
    return 1;
  }
    
    // How many threads to create
    int N = atoi(argv[1]);  

    // Allocate memory for the arrays on the heap
    pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));
    struct data* d = (struct data*) malloc(N * sizeof(struct data));

    // Create threads and do work
    for (int i=0; i<N; i++)
    {
        // Store data
        d[i].index = i;

        // Create thread
        pthread_create(&threads[i], NULL, thread_func, &d[i]);
    }


    // Join all threads
    for (int i; i<N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Free memory
    free(d);
    free(threads);

    printf("\n\n");
    return 0;
}