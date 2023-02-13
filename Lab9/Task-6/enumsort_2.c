/**********************************************************************
 * Enumeration sort
 * 
 * gcc -Wall -O3 -o enumsort_2 enumsort_2.c -lpthread
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS	4
#define len 100000

int p = 10;   // How many elements each thread should process at a time


static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

double indata[len], outdata[len];

void *findrank(void *arg)
{
	int rank,i;
  int idx;
	
  long* el = (long*) arg;
  
  for (idx=0; idx<p; idx++){
    long j= el[idx];
    rank=0;
    for (i=0;i<len;i++)
      if (indata[i]<indata[j]) rank++;
    outdata[rank]=indata[j];
	}
  
  pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  int i, j, t;
 
  // on stack
  long el[p];  //el is an array of length p instead of just one element

  // on heap
  // long* el = (long*) malloc(p * sizeof(long));

  void *status;
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

  // Generate random numbers
  for (i=0;i<len;i++){
      indata[i]=drand48();
      outdata[i]=-1.0;
  }

  // Enumeration sort
  double startTime = get_wall_seconds();
  for (j=0;j<len;j+=NUM_THREADS)
    {
		for(t=0; t<NUM_THREADS; t++)
    {
      for (i=0;i<p;i++)
      {el[i] = j+t+i;}
      
		  pthread_create(&threads[t], &attr, findrank, (void *)el);
    }
		
		for(t=0; t<NUM_THREADS; t++) 
			pthread_join(threads[t], &status);
    }
  double timeTaken = get_wall_seconds() - startTime;
  printf("Time: %f  NUM_THREADS: %d\n", timeTaken, NUM_THREADS);

  // Check results, -1 implies data same as the previous element
    for (i=0; i<len-1; i++)
      if (outdata[i]>outdata[i+1] && outdata[i+1]>-1)
	printf("ERROR: %f,%f\n", outdata[i],outdata[i+1]);


  // Clean up
  //free(el);

  return 0;
}
