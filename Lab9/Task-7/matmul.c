#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 16

pthread_mutex_t m;

double **A,**B,**C;
int n;

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}


void* mult(void* arg)   // idx should be (0,1,2,3)
{
  int i,j,k;
  int idx = *(int*)arg; 
  double temp;
  
  // Matrix multiplication
  for(i=idx; i<n; i=i+NUM_THREADS)  // i = index of every 4th row in A
    {
      for(j=0; j<n; j++)           // j = index of every column in B
        {
          temp =0;
          for (k=0; k<n; k++)        // k = index for stepping through elements in rows/columns in A and B
          {
            temp += A[i][k] * B[k][j];
          }
          pthread_mutex_lock(&m);
          C[i][j]= temp;
          pthread_mutex_unlock(&m);
        }
      }
  return NULL;
}


int main(int argc, char *argv[]) {
  int i, j, k,t;

  pthread_mutex_init(&m, NULL);

  if(argc != 2) {
    printf("Please give one argument: the matrix size n\n");
    return -1;
  }

  n = atoi(argv[1]);

  //Allocate and fill matrices
  A = (double **)malloc(n*sizeof(double *));
  B = (double **)malloc(n*sizeof(double *));
  C = (double **)malloc(n*sizeof(double *));
  for(i=0;i<n;i++){
    A[i] = (double *)malloc(n*sizeof(double));
    B[i] = (double *)malloc(n*sizeof(double));
    C[i] = (double *)malloc(n*sizeof(double));
  }

  for (i = 0; i<n; i++)
    for(j=0;j<n;j++){
      A[i][j] = rand() % 5 + 1;
      B[i][j] = rand() % 5 + 1;
      C[i][j] = 0.0;
    }



  printf("Doing matrix-matrix multiplication...\n");
  double startTime = get_wall_seconds();

  pthread_t threads[NUM_THREADS];
  int args[NUM_THREADS];

  // Create threads
  for(t=0; t<NUM_THREADS; t++)
  {
    args[t] = t;
    pthread_create(&threads[t], NULL, mult, &args[t]);
  }

  // Join threads
  for(t=0; t<NUM_THREADS; t++) 
  {
    pthread_join(threads[t], NULL);
  }

  double timeTaken = get_wall_seconds() - startTime;
  printf("Elapsed time: %f wall seconds\n", timeTaken);

  // Correctness check (let this part remain serial)
  printf("Verifying result correctness for a few result matrix elements...\n");
  int nElementsToVerify = 5*n;
  double max_abs_diff = 0;
  for(int el = 0; el < nElementsToVerify; el++) {
    i = rand() % n;
    j = rand() % n;
    double Cij = 0;
    for(k = 0; k < n; k++)
      Cij += A[i][k] * B[k][j];
    double abs_diff = fabs(C[i][j] - Cij);
    if(abs_diff > max_abs_diff)
      max_abs_diff = abs_diff;
  }
  printf("max_abs_diff = %g\n", max_abs_diff);
  if(max_abs_diff > 1e-10) {
    for(i = 0; i < 10; i++)
      printf("ERROR: TOO LARGE DIFF. SOMETHING IS WRONG.\n");
    return -1;
  }
  printf("OK -- result seems correct!\n");

  // Destroy mutext
  pthread_mutex_destroy(&m);

  // Free memory
  for(i=0;i<n;i++){
    free(A[i]);
    free(B[i]);
    free(C[i]);
  }
  free(A);
  free(B);
  free(C);

  return 0;
}
