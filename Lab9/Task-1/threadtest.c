#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int N = 8; // Size of block


void* the_thread_func(void* arg) {
  
  int* p = (int*) malloc(N * sizeof(int)); // p is a pointer to block of ints in the heap

  // Put integers 0->7 in the memory block
  for (int i=0; i<N; i++)
  {
    p[i] = i;
  }
  
  pthread_exit(p);
  return NULL;
  //return p;
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  if(pthread_create(&thread, NULL, the_thread_func, NULL) != 0) {
    printf("ERROR: pthread_create failed.\n");
    return -1;
  }

  printf("This is the main() function after pthread_create()\n");

  
  int* ptr;  // ptr is a pointer for the results


  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  if(pthread_join(thread, (void**)&ptr) != 0) {
    printf("ERROR: pthread_join failed.\n");
    return -1;
  }

  printf("Result from thread_func:\n");
  for (int i=0; i<N; i++)
  {
    printf("%d, ", ptr[i]);
  }
  
  
  
  // Free the memory of the original pointer
  free(ptr);
  
  
  
  printf("\n");

  return 0;
}
