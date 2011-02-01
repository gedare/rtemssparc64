/*
 *  Sample code for the DIS Neighborhood Stressmark
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
#include <stdlib.h>
#include <time.h>
#include<assert.h>
#include "DISstressmarkRNG.h"
#include "utili.h"

/* 
 * main()
 */
int main(int argc, char *argv[])
{
  long int seed;
  int dimension;
  int numberLines;
  int minThickness;
  int maxThickness;
  int distanceShort;
  int distanceLong;
  int bitDepth;
  int maxPixel;
  Pixel *image;
  Neighborhood values;

  time_t beginTime;
  time_t endTime;

  /* GAB: modified to read from a file instead of GAB_fp */
  FILE *GAB_fp;
  if ((GAB_fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: Unable to open %s\n",argv[0],argv[1]);
    exit(1);
  }

  fscanf(GAB_fp, "%ld %d %d %d %d %d %d %d", 
	&seed, &bitDepth, &dimension, &numberLines,
	&minThickness, &maxThickness,
	&distanceShort, &distanceLong);

  assert((seed >= MIN_SEED) && (seed <= MAX_SEED));
  assert((dimension > 0) && (dimension <= MAX_DIMENSION));
  assert((numberLines > 0) && (numberLines <= MAX_NUMBER_LINES));
  assert((minThickness > 0) && (minThickness < dimension));
  assert((maxThickness >= minThickness) && (maxThickness < dimension));
  assert((distanceShort > 0) && (distanceShort < dimension));
  assert((distanceLong > 0) && (distanceLong < dimension));
  assert((bitDepth >= MIN_BIT_DEPTH) && (bitDepth <= MAX_BIT_DEPTH));

  randInit(seed);
  maxPixel = (1 << bitDepth) - 1;
  image = createImage(dimension, maxPixel, numberLines,
		      minThickness, maxThickness);
  assert (image != NULL);

  beginTime = time(NULL);

  neighborhoodCalculation(image, dimension,
			  distanceShort, distanceLong, &values, maxPixel);

  endTime = time(NULL);

  printf(" end time is %d\n", endTime);

  fprintf(stdout, "%9.4e %9.4e %9.4e %9.4e %9.4e %9.4e %9.4e %9.4e", 
	  values.distShort.deg0.entropy,
	  values.distShort.deg0.energy,
	  values.distShort.deg45.entropy,
	  values.distShort.deg45.energy,
	  values.distShort.deg90.entropy,
	  values.distShort.deg90.energy,
	  values.distShort.deg135.entropy,
	  values.distShort.deg135.energy);

  fprintf(stdout,"%9.4e %9.4e %9.4e %9.4e %9.4e %9.4e %9.4e %9.4e",
	  values.distLong.deg0.entropy,
	  values.distLong.deg0.energy,values.distShort.deg45.entropy,
	  values.distLong.deg45.energy,
	  values.distLong.deg90.entropy,
	  values.distLong.deg90.energy,
	  values.distLong.deg135.entropy,
	  values.distLong.deg135.energy);

  fprintf(stderr, "time for neghborhood stressmark = %f\n",
	  difftime(endTime, beginTime));
	  
  free((Pixel *)image);
  fclose(GAB_fp);
  return (SUCCESS);
  }



   
