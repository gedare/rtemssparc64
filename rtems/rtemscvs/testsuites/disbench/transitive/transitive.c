/*
 *  Sample code for the DIS Pointer Stressmark
 *
 * This source code is the completely correct source code based on 
 * the example codes provided by Atlantic Aerospace Division, Titan 
 * Systems Corporation, 2000.
 * 
 * If you just compile and generate the executables from this source 
 * code, this code would be enough. However, if you wish to get a complete 
 * understanding of this stressmark, it is strongly suggested that you
 * read the Benchmark Analysis and Specifications Document Version 1.0
 * before going on since the detailed comments are given in this documents.
 * the comments are not repeated here.
 */

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include "../common/DISstressmarkRNG.h"

#include "../../common/allow.h"


#define MIN_VERTICES 8
#define MAX_VERTICES 16384
#define MIN_EDGES 0
#define MAX_EDGES 268435456
#define MIN_SEED -2147483647
#define MAX_SEED -1
#define NO_PATH 2147483647

#define MIN_EDGS 0
#define MAX_EDGE 255

/* 
 * main()
 */

int transitive_main(int argc, char *argv[]){
  unsigned int *din, *dout;
  unsigned int n;
  unsigned int m;
  unsigned int i, j, k;
  int seed;

  time_t startTime;
  unsigned int sum;
  /* GAB: modified to read from a file instead of GAB_fp */
  FILE *GAB_fp;
  if ((GAB_fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: Unable to open %s\n",argv[0],argv[1]);
    exit(1);
  }


  fscanf(GAB_fp,"%d %d %d", &n, &m, &seed);

  assert((n >= MIN_VERTICES) && (n <= MAX_VERTICES));
  assert((m >= MIN_EDGES) && (m <= MAX_EDGES));
  assert (m <= n*n);
  assert ((seed >= MIN_SEED) && (seed <= MAX_SEED));

 din = (unsigned int *)mymalloc(n*n*sizeof(unsigned int));
  assert(din);
 dout = (unsigned int *)mymalloc(n*n*sizeof(unsigned int));
  assert(dout);

 for (i=0; i<n*n; i++){
   *(din + i) = NO_PATH;
   *(dout + i) = NO_PATH;
 }

  randInit(seed);
  for (k=0; k<m; k++){
    i = randInt(0, n-1);
    j = randInt(0, n-1);
    *(din + j*n + i) = randInt(MIN_EDGES, MAX_EDGES);
  }
  
  startTime = time(NULL);

  for (k=0; k<n; k++){
  unsigned int old;
  unsigned int new1;
  unsigned int *dtemp;
    
    for (i=0; i<n; i++){
      for (j=0; j<n; j++){
	old = *(din + j*n + i);
    /*    printf(" din+jn + i) = %d \n", din + j*n + i);
   */ 
	new1 = *(din + j*n + k) + *(din + k*n + i);
	*(dout + j*n + i) = (new1 < old ? new1: old);
/*
        printf(" dout +jn + i) = %d \n", dout + j*n + i);
*/
	assert (*(dout + j*n + i) <= NO_PATH);
	assert (*(dout + j*n + i) <= *(din + j*n + i));

      }
    }
      dtemp = dout;
      dout = din;
      din = dtemp;
    }

    startTime = time(NULL) - startTime;
    
    for (j=0; j<n; j++){
      sum = 0;
      for (i=0; i<n; i++){
	if (*(din + j*n + i) != NO_PATH)
	  sum += *(din + j*n + i);
      }
    fprintf(stdout, "%u\n", sum);
    }
    for (i=0; i<n; i++){
      sum = 0;
      for (j=0; j<n; j++){
	if (*(din + j*n + i) != NO_PATH)
	    sum += *(din+j*n+i);
      }
      
      fprintf(stdout, "%u\n", sum);
    }

    fprintf(stdout, " total time = %u seconds. \n", (unsigned int)startTime);
    free(din);
    free(dout);
    return(0);
  }
    

	

    
 
  
