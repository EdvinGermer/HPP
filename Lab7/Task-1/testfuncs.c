#include "testfuncs.h"
#include <stdio.h>

void f_std(const double * __restrict a, 
	   const double * __restrict b, 
	   double * __restrict c, int N) {
  int i;
  double x = 0.3;
  for(i = 0; i < N; i++) {
    c[i] = a[i]*a[i] + b[i] + x;
  }
}

void f_opt(const double * __restrict a, 
	   const double * __restrict b, 
	   double * __restrict c, int N) {
  int i;
  double x = 0.3;

  // N = N1 = 200;
  //int fac = 0;
  for(i = 0; i < N; i+=1)    // N-fac, i+=fac
  {
    c[i] = a[i]*a[i] + b[i] + x;
    
    /*
    c[i+1] = a[i+1]*a[i+1] + b[i+1] + x;
    c[i+2] = a[i+2]*a[i+2] + b[i+2] + x;
    c[i+3] = a[i+3]*a[i+3] + b[i+3] + x;
    c[i+4] = a[i+4]*a[i+4] + b[i+4] + x;
    c[i+5] = a[i+5]*a[i+5] + b[i+5] + x;
    c[i+6] = a[i+6]*a[i+6] + b[i+6] + x;
    c[i+7] = a[i+7]*a[i+7] + b[i+7] + x;
    c[i+8] = a[i+8]*a[i+8] + b[i+8] + x;
    c[i+9] = a[i+9]*a[i+9] + b[i+9] + x;
    c[i+10] = a[i+10]*a[i+10] + b[i+10] + x;
    c[i+11] = a[i+11]*a[i+11] + b[i+11] + x;
    */
    
  }


  for (; i < N; i++)  // If there is a remainder
  {
    c[i] = a[i]*a[i] + b[i] + x;
  }


}

