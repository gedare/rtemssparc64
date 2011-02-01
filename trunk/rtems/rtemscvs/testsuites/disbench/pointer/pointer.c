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
#include "DISstressmarkRNG.h"

#define MIN_FIELD_SIZE 16
#define MAX_FIELD_SIZE 16777216
#define MIN_WINDOW_SIZE 1
#define MAX_WINDOW_SIZE 15
#define MIN_HOP_LIMIT 1

#define MAX_HOP_LIMIT 4294967295U

#define MIN_SEED -2147483647
#define MAX_SEED -1
#define MIN_THREADS 1
#define MAX_THREADS 256

/*
 * main()
 */
int main(int argc, char *argv[]){

  unsigned int *field;
  unsigned int f;
  unsigned int w;
  unsigned int maxhops;
  int seed;
  unsigned int n;

  clock_t startTime;

  struct threadS{
    unsigned int initial;
    unsigned int minStop;
    unsigned int maxStop;
    unsigned int hops;
  }*thread;

  unsigned int l;

  /* GAB: modified to read from a file instead of GAB_fp */
  FILE *GAB_fp;
  if ((GAB_fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: Unable to open %s\n",argv[0],argv[1]);
    exit(1);
  }

 fscanf(GAB_fp, "%u %u %u %d %u", 
	&f, &l, &maxhops, &seed, &n);
 assert ((f >= MIN_FIELD_SIZE) && (f <= MAX_FIELD_SIZE));
 w = (unsigned int) l;
 assert ((w >= MIN_WINDOW_SIZE) && (w <= MAX_WINDOW_SIZE));
 assert (w % 2 == 1);
 assert (f > w);
 assert ((maxhops >= MIN_HOP_LIMIT) && (maxhops <= MAX_HOP_LIMIT));
 assert ((seed >= MIN_SEED) && (seed <= MAX_SEED));

 assert ((n >= MIN_THREADS) && (n <= MAX_THREADS));
 thread = (struct threadS *)malloc(n*sizeof(struct threadS));
 assert(thread);

 for (l=0; l<n; l++){
   fscanf(GAB_fp, "%u %u %u", 
	  &(thread[l].initial), &(thread[l].minStop), &(thread[l].maxStop));
   assert ((thread[l].initial >= 0) && (thread[l].initial < f));
   assert ((thread[l].minStop >= 0) && (thread[l].minStop < f));
   assert ((thread[l].maxStop >= 0) && (thread[l].maxStop < f));
 }

 field = (unsigned int *)malloc(f*sizeof(int));
 assert(field);

 randInit(seed);
 for (l=0; l<f; l++){
   field[l] = randInt(0, f-w-1);
}

startTime = time(NULL);
clock();

for (l=0; l<n; l++)
{
 unsigned int index;
 unsigned int minStop, maxStop;
 unsigned int hops;
 
 hops = 0;
 minStop = thread[l].minStop;
 maxStop = thread[l].maxStop;
 index = thread[l].initial;
 while ((hops < maxhops) &&
        (!((index >= minStop) &&
         (index < maxStop)))){

 unsigned int ll, lll;
 unsigned int max, min;
 unsigned int partition;
 unsigned int high;
 
 partition = field[index];
 max = MAX_FIELD_SIZE;
 min = 0;
 high = 0;
 
 for (ll=0; ll<w; ll++){
   unsigned int balance;
   unsigned int x;
    x = field[index+ll];

   if (x > max) high++;
   else if (x > min){ /* start else* */
     partition = x;
     balance = 0;
     for (lll=ll+1; lll<w; lll++){
       if (field[index+lll] > partition) balance++;
     }/* end for loop */

     if (balance+high == w/2) break;
     else if (balance+high > w/2){
          min = partition;
     }/* end if */
     else { 
          max = partition;
          high++;
     }/* end else */
  }
   if (min == max) break;
 } /* end else* */
  index = (partition+hops)%(f-w);
  hops++;
 }/* end loop ll */
  thread[l].hops = hops;
} /* end while */

 startTime = time(NULL) - startTime;

 for (l=0; l<n; l++){
  fprintf(stdout, "%u hops on thread %d\n", thread[l].hops, l);
 }

 fprintf(stderr, "total time = %u seconds.\n", (unsigned int)startTime);
 free (field);
 free (thread);
 fclose(GAB_fp);
 return 0;
}
