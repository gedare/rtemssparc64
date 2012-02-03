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



#define MIN_FIELD_SIZE 16
#define MAX_FIELD_SIZE 16777216
#define MIN_SEED -2147483647
#define MAX_SEED -1
#define MIN_MOD_OFFSET 0
#define MAX_MOD_OFFSET 65535
#define MIN_TOKENS 1
#define MAX_TOKENS 256
#define MIN_TOKEN_LENGTH 1
#define MAX_TOKEN_LENGTH 8
#define MIN_TOKEN_VALUE 0
#define MAX_TOKEN_VALUE 255
#define MAX_SUBFIELDS 256

/*
 * main()
 */

int field_main(int argc, char *argv[]){
  
  unsigned char *field;
  unsigned int f;
  int seed;
  int mod_offset;
  unsigned int n;

  time_t startTime;

  /* GAB: make static to get off stack */
  static struct tokenS{
  unsigned char delimiter[MAX_TOKEN_LENGTH];
  unsigned char length; 
    struct statisticS{
      unsigned int count;
      unsigned char min;
      unsigned char sum;
    }stat[MAX_SUBFIELDS];
    unsigned char subfields;
  }token[MAX_TOKENS];

  unsigned int l;

  /* GAB: modified to read from a file instead of stdin */
  FILE *GAB_fp;
  if ((GAB_fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: Unable to open %s\n",argv[0],argv[1]);
    exit(1);
  }

  fscanf(GAB_fp, "%d %d %d %d", &f, &seed, &mod_offset, &n);

  assert((f >= MIN_FIELD_SIZE) && (f <= MAX_FIELD_SIZE));
  assert((seed >= MIN_SEED) && (seed <= MAX_SEED));
  assert((mod_offset >= MIN_MOD_OFFSET) && (mod_offset <= MAX_MOD_OFFSET));
  assert((n >= MIN_TOKENS) && (n <= MAX_TOKENS));
    for (l=0; l<n; l++){
      int x;
      int index;
      index = 0;
      fscanf(GAB_fp,"%x", &x);
      while(x!=0){
	assert((x >= MIN_TOKEN_VALUE) && (x <= MAX_TOKEN_VALUE));
	  token[l].delimiter[index] = (unsigned char )x;
	index++;
	fscanf(GAB_fp,"%x", &x);
      }
      assert((index >= MIN_TOKEN_LENGTH) && (index <= MAX_TOKEN_LENGTH));
	token[l].length = index;
    }

  field = (unsigned char*)mymalloc(f*sizeof(unsigned char));
  assert(field);

  randInit(seed);
  for (l =0; l<f; l++){
    field[l] = randInt(MIN_TOKEN_VALUE, MAX_TOKEN_VALUE);
  }
  
  startTime = time(NULL);
  
  for (l =0; l<n; l++){
    unsigned int index;
    
    token[l].subfields = 0;
    token[l].stat[0].count = 0;
    token[l].stat[0].sum = 0;
    token[l].stat[0].min = MAX_TOKEN_VALUE;

    index = 0;
    while ((index < f) && (token[l].subfields < MAX_SUBFIELDS)){
      unsigned char offset;
      offset = 0;
      while ((field[index+offset] == token[l].delimiter[offset]) &&
	     (offset < token[l].length)){
	offset++;
      }

      if (offset == token[l].length){
	for (offset=0; offset<token[l].length; offset++){
	  field[index+offset] = (field[index+offset] + 
				 field[(index+offset+mod_offset) % f])
	                         %(MAX_TOKEN_VALUE+1);
	}
	index += token[l].length-1;
	token[l].subfields++;
	token[l].stat[token[l].subfields].count = 0;
	token[l].stat[token[l].subfields].sum = 0;
	token[l].stat[token[l].subfields].min = MAX_TOKEN_VALUE;
      }	     	     	       
		    
	else{
       	token[l].stat[token[l].subfields].count++;
	token[l].stat[token[l].subfields].sum += field[index];
	if (token[l].stat[token[l].subfields].min > field[index])
	token[l].stat[token[l].subfields].min = field[index];
	}	     
	index++;
    }
    }

    startTime = time(NULL) - startTime;

    for (l = 0; l< n; l++){
      unsigned int ll;
      fprintf(stdout, "%d subfields for token %d \n", token[l].subfields, l);
      for ( ll =0; ll<token[l].subfields; ll++)
	fprintf(stdout, "subfields %d:\tcount = %d\tmin= %x\tsum= %x\n",
		ll, token[l].stat[ll].count, 
		token[l].stat[ll].min, token[l].stat[ll].sum);
    }
    fprintf(stdout, "total time = %d seconds.\n", (int)startTime);
    free(field);
    fclose(GAB_fp);
    return(0);
  }
