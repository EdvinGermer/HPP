#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// Number of main and secondary threads
int N = 2; 


void* thread_func(void* args) 
{
    printf("        Tertiary thread created by a thread\n");
}


void* thread_creator(void* args)  
{
   
   printf("    Secondary thread created in main\n");

   // Allocate memory for secondary threads
    pthread_t* secondary_threads = (pthread_t*) malloc(N * sizeof(pthread_t));

    // Create secondary threads 
    for (int i=0; i<N; i++)
    {
        // Create thread
        pthread_create(&secondary_threads[i], NULL, thread_func, NULL);
    }

    // Join all threads
    for (int i=0; i<N; i++)
    {
        pthread_join(secondary_threads[i], NULL);
    }

    // Free memory
    free(secondary_threads);
}


int main()
{
    printf("\n\nMain thread\n");

    // Allocate memory for the arrays on the heap
    pthread_t* main_threads = (pthread_t*) malloc(N * sizeof(pthread_t));

    // Create threads and do work
    for (int i=0; i<N; i++)
    {
        // Create thread
        pthread_create(&main_threads[i], NULL, thread_creator, NULL);   // d is a pointer
    }

    // Join all main threads
    for (int i=0; i<N; i++)
    {
        pthread_join(main_threads[i], NULL);
    }


    // Free memory
    free(main_threads);

    printf("\n\n");
    return 0;
}