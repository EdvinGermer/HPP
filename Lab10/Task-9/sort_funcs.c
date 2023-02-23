#include <stdlib.h>
#include "sort_funcs.h"
#include <omp.h>
#include <stdio.h>

int count=0;

void bubble_sort(intType* list, int N) {
  int i, j; 
  for(i = 0; i < N-1; i++)
    for(j = 1+i; j < N; j++) {
      if(list[i] > list[j]) {
	// Swap  
	intType tmp = list[i];
	list[i] = list[j];
	list[j] = tmp;
      }
    }
}

void merge_sort(intType* list_to_sort, int start, int end, int nThreads) {
  
  int N = end-start;

  if(N == 1) {
    // Only one element, no sorting needed. Just return directly in this case.
    return;
  }

  int middle = start + (end - start) / 2;;

  // Enable nested parallelism
  omp_set_nested(1);
 

  // Allocate new list
  intType* list = (intType*)malloc(N * sizeof(intType));
if (list == NULL) {
  printf("Failed to allocate memory.\n");
  return;
}

  
  // Copy list_to_sort to new list
  int i;
  for(i = 0; i < N; i++) 
    list[i] = list_to_sort[start+i];

  // Sort list1 and list2
  if (nThreads > 1)
  {
    #pragma omp parallel num_threads(2)
    {
      #pragma omp single nowait
       {merge_sort(list, 0, middle, nThreads/2);}
 
      #pragma omp single nowait
       {merge_sort(list, middle, N, nThreads/2);}
    }
  }
  else
  {
    merge_sort(list, 0, middle, 1);
    merge_sort(list, middle, end, 1);
  }

  // Merge like a zipper
  int i1 = 0;
  int i2 = middle;
  i = start;
  while(i1 < middle && i2 < end)
  {
    if(list[i1] < list[i2]) {
      list_to_sort[i] = list[i1];
      i1++;
    }
    else {
      list_to_sort[i] = list[i2];
      i2++;
    }
    i++;
  }

  // Merge the rest
  while(i1 < middle)
  {
    list_to_sort[i] = list[i1];
    i++;
    i1++;
  }
    
  while(i2 < end)
  {
    list_to_sort[i] = list[i2]; 
    i++;
    i2++;
  }

  if(list != NULL) {
  free(list);
  list = NULL;
}

}

