#include <stdio.h>
#include <pthread.h>

// gcc -o threadtest1 threadtest1.c -lpthread

void* the_thread_func(void* arg) {
  /* Do something here? */
  printf("the_thread_func() starting doing some work.\n");
  long int i;
  double sum;
  for(i = 0; i < 1000000000; i++)
    sum += 1e-7;
  printf("Result of work in the_thread_func(): sum = %f\n", sum);

  return NULL;
}


void* the_thread_func_b(void* arg) {
  /* Do something here? */
  printf("the_thread_func_b() starting doing some work.\n");
  long int i;
  double sum;
  for(i = 0; i < 1000000000; i++)
    sum += 1e-7;
  printf("Result of work in the_thread_func_b(): sum = %f\n", sum);

  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  pthread_t thread_b;

  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, NULL);
  pthread_create(&thread_b, NULL, the_thread_func_b, NULL);

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */
  printf("main() starting doing some work.\n");
  long int i;
  double sum;
  for(i = 0; i < 1000000000; i++)
    sum += 1e-7;
  printf("Result of work in main(): sum = %f\n", sum);

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join() on the first thread.\n");
  pthread_join(thread, NULL);

  printf("the main() function now calling pthread_join() on the second (b) thread.\n");
  pthread_join(thread_b, NULL);

  return 0;
}
