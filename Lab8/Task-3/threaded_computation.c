#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

const long int N1 = 100000000;
const long int N2 = 800000000-N1;

void* the_thread_func(void* arg) {
  double get_wall_seconds();
  double time2 =  get_wall_seconds();
  long int i;
  long int sum = 0;
  for(i = 0; i < N2; i++)
    sum += 7;
  /* OK, now we have computed sum. Now copy the result to the location given by arg. */
  long int * resultPtr;
  resultPtr = (long int *)arg;
  *resultPtr = sum;
  printf("thread function took %7.3f wall seconds.\n", get_wall_seconds()-time2);
  return NULL;
}


double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}


int main() {
  double time1;
  double get_wall_seconds();
  
  printf("This is the main() function starting.\n");

  long int thread_result_value = 0;

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, &thread_result_value);


  printf("This is the main() function after pthread_create()\n");

  time1 = get_wall_seconds();
  long int i;
  long int sum = 0;
  for(i = 0; i < N1; i++)
    sum += 7;
  printf("main function took %7.3f wall seconds.\n", get_wall_seconds()-time1);



  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);

  printf("sum computed by main() : %ld\n", sum);
  printf("sum computed by thread : %ld\n", thread_result_value);
  long int totalSum = sum + thread_result_value;
  printf("totalSum : %ld\n", totalSum);

  return 0;
}
