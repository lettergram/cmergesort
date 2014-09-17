/**
 * Modified version of Merge Sort Algorithm
 * Written by Austin Walters
 *
 * Origionally written for CS241@UIUC in Spring 2013
 * Last Modified Sep 2014 for austingwalters.com
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

typedef struct _variable_t{

  int * array;    // pointer to array of numbers collected via stdin
  size_t per_seg; // number of slots FOR NUMBERS per segments

}variable_t;


volatile int threads_running = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int * sortedArray; // Pointer to sorted array

/**
 * Compare function used in qsort (void *sort function)
 */
int compare (const void * a, const void * b){
  if ( *(int*)a <  *(int*)b ){ return -1; }
  if ( *(int*)a >  *(int*)b ){ return 1; }
  return 0;
}

/**
 * Sort portion of mergesort, sorts array using qsort (thread safe)
 */
void * sort(void * t){

  variable_t variable = *(variable_t *)t;
  qsort(variable.array, variable.per_seg, sizeof(int), compare);
  pthread_mutex_lock(&mutex);
  threads_running--;
  pthread_mutex_unlock(&mutex);
  fprintf(stderr, "I just sorted %zu elements!\n", variable.per_seg);
  return t;
}

/**
 * Merge portion of the mergesort, merges left and right arrays
 */
int merge(int *arrayL, int * arrayR, size_t lenL, size_t lenR){
  
  unsigned int L = 0;
  unsigned int R = 0;
  unsigned int k = 0;
  int d = 0;

  while(L < lenL && R < lenR){

    if(arrayL[L] < arrayR[R]){
      sortedArray[k] = arrayL[L];
      L++;
      k++;
    }else if(arrayR[R] < arrayL[L]){
      sortedArray[k] = arrayR[R];
      R++;
      k++;
    }else{
      sortedArray[k] = arrayR[R];
      k++;
      sortedArray[k] = arrayL[L];
      L++;
      R++;
      k++;
      d++;
    }
  }
  while(L < lenL){
    sortedArray[k] = arrayL[L];
    L++;
    k++;
  }

  while(R < lenR){
    sortedArray[k] = arrayR[R];
    R++;
    k++;
  }

  return d;
}

/**
 * Grabs the users unsorted inputlist, returns it in array form
 */
int * unsortedUserList(){
  
  char * line = NULL;
  size_t len = 0;
  int i = 0;                                // i = number of numbers
  int size = 10;                            // size is the number of slots for int
  int * array = malloc(sizeof(int)*size);   // array[size] stores the inputs
  
  while(-1 != getline(&line, &len, stdin)){
    if(size > i){
      array[i] = atoi(line);
    }else{
      size *= 2;
      array = realloc(array, sizeof(int)*size);
      array[i] = atoi(line);
    }
    i++;
    free(line);
    line = NULL;
  }
  return array;
}

/**
 * Main Function (loop)
 * 
 * argv[1] - segment count
 *
 * TODO: Pretty ugly and should be refactored (heavily!!!)
 *       I've gotten way better at coding IMO...
 */
int main(int argc, char **argv){
  
  int segment_count = atoi(argv[1]);
  
  int i = 0;
  int * array = unsortedUserList();
  
  /* Thread Variables */
  pthread_t * threads = malloc(sizeof(pthread_t) * i);
  variable_t * thread_arg = malloc(sizeof(variable_t) * i);
  /* End of Thread Variables */

  int j = 0;
  int n = i;
  int k = 0;
  int per_seg = 0;

  if(n % segment_count == 0){
    per_seg = i / segment_count;
  }else{
    per_seg = (i / segment_count) + 1;
  }
  
  n -= per_seg;
  
  while(k < segment_count){
  
    thread_arg[k].array = &array[j];
    thread_arg[k].per_seg = per_seg;
    
    pthread_mutex_lock(&mutex);
    threads_running++;
    pthread_mutex_unlock(&mutex);
    
    pthread_create(&threads[k], NULL, sort, (void *)&thread_arg[k]);
    
    j += per_seg;
    
    if(n % segment_count == 0){
      per_seg = i / segment_count;
    }else{
      per_seg = (i / segment_count) + 1;
    }
    if(n < per_seg){
      per_seg = n;
    }
    n -= per_seg;
    k++;
  }
  
  int duplicates = 0;
  j = 0;
  n = 1;
  
  while(threads_running > 0){ sleep(1); } // forces waiting
  
  sortedArray = malloc(i*sizeof(int));

  while(segment_count){
    while(j+n < atoi(argv[1])){
      
      pthread_join(threads[j + n], NULL);
      
      duplicates = merge(thread_arg[j].array, thread_arg[j + n].array, thread_arg[j].per_seg, thread_arg[j + n].per_seg);
      
      fprintf(stderr, "I just merged %d and %d elements with %d duplicate elements.\n", (int)thread_arg[j].per_seg, (int)thread_arg[j + n].per_seg, duplicates);
      
      thread_arg[j].per_seg += thread_arg[j + n].per_seg;
      
      memcpy(thread_arg[j].array, sortedArray, (thread_arg[j].per_seg * sizeof(int)));
      
      j += 2*n;
    }
    n+=n;
    j = 0;
    segment_count = (segment_count - (segment_count % 2))/2;
  }
  
  pthread_join(threads[0], NULL);

  j = 0;
  while(j < i){printf("%d\n", array[j]); j++;}

  free(sortedArray);
  free(array);
  free(threads);
  free(thread_arg);
  return 0;
}