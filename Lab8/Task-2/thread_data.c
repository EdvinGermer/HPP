#include <stdio.h>
#include <pthread.h>

// gcc -o thread_data thread_data.c -lpthread

void* the_thread_func(void* arg) {
  /* Do something here? */
  double *data = (double *) (arg);  // Type case input argument to a double pointer
  printf("     The 1st data: %f\n", data[0]);
  printf("     The 2nd data: %f\n", data[1]);
  printf("     The 3rd data: %f\n", data[2]);

  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  double data_for_thread[3];
  data_for_thread[0] = 5.7;
  data_for_thread[1] = 9.2;
  data_for_thread[2] = 1.6;

  double data_for_thread_b[3];
  data_for_thread_b[0] = 3.5;
  data_for_thread_b[1] = 6.1;
  data_for_thread_b[2] = 7.8;

  /* Start thread. */
  pthread_t thread;
  pthread_t thread_b;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, data_for_thread);
  pthread_create(&thread_b, NULL, the_thread_func, data_for_thread_b);

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);

  return 0;
}
