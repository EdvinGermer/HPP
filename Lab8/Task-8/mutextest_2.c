#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t sum_mutex;
long int sum = 0;

int N = 100000;

void* the_thread_func(void* arg) {

  pthread_mutex_lock(&sum_mutex);
  for(int i = 1; i <= N; ++i)
  	sum += 1;
  pthread_mutex_unlock(&sum_mutex);

  return NULL;
}

int main(int argc, char **argv) {

  if(argc != 2) {printf("Usage: %s N\n", argv[0]); return -1; }

  printf("This is the main() function starting.\n");

  int N = atoi(argv[1]);

  pthread_mutex_init(&sum_mutex, NULL);

  /* Start thread. */
  printf("the main() function now calling pthread_create().\n");
  pthread_t threads[N];
  for(int i = 0; i < N; i++)
    pthread_create(&threads[i], NULL, the_thread_func, NULL);

  printf("This is the main() function after pthread_create()\n");



  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  for(int i = 0; i < N; i++)
    pthread_join(threads[i], NULL);

  printf("sum = %ld\n", sum); 


  pthread_mutex_destroy(&sum_mutex);
  return 0;
}
