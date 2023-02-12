/**********************************************************************
 * This program calculates pi in parallel using C 
 *
 **********************************************************************/
#include <stdio.h>
#include <pthread.h>

int NUM_THREADS = 16;
double res=0.0;
pthread_mutex_t m;

struct args{
  int idx;
  double dx;
  int low;
  int upp;
};

void* thread_func(void* input)
{
  struct args* a = (struct args*) input;

  int idx = a->idx;
  double dx = a->dx;
  int low = a->low;
  int upp = a->upp;

  printf("Thread %d has lower bound = %d, upper bound = %d\n",idx, low, upp);

  int i;
  double x;
  double sum = 0.0;

  for (i = low; i <= upp; i++) { 
    x = dx*(i - 0.5);
    sum += dx*4.0/(1.0 + x*x);
  }

  printf("  Thread %d calcualtes the sum = %f\n",idx, sum);

  pthread_mutex_lock(&m);
  res += sum;
  pthread_mutex_unlock(&m);
}


int main(int argc, char *argv[])
{
  int t;
  const double intervals = 500000000;
  double dx;
  pthread_t threads[NUM_THREADS];
  struct args d[NUM_THREADS];

  pthread_mutex_init(&m, NULL);

  dx  = 1.0/intervals;
  //printf("dx = %f\n", dx);

  for(t=0; t<NUM_THREADS; t++)
  {
    int l = (intervals/NUM_THREADS)*t+1;
    int u = (intervals/NUM_THREADS)*(t+1);
    
    d[t].idx=t;
    d[t].dx=dx;
    d[t].low=l;
    d[t].upp=u;

    pthread_create(&threads[t], NULL, thread_func, &d[t]); 
  }

  // Wait for threads to finish
  for(t=0; t<NUM_THREADS; t++)
    pthread_join(threads[t], NULL);

  // Print results
   printf("PI is approx. %.16f\n",  res);

  // Cleanup
  pthread_mutex_destroy(&m);


  // Done!
  return 0;
}
