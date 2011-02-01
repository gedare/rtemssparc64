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

#include <math.h>

#define TRUE 1
#define FALSE !TRUE
#define SUCCESS TRUE
#define ERROR FLASE

#define MIN_PIXEL 0
#define MAX_DIMENSION 32768
#define MIN_SEED  -2147483647
#define MAX_SEED  -1
#define MAX_NUMBER_LINES 65536
#define MIN_BIT_DEPTH    7
#define MAX_BIT_DEPTH    15

typedef struct {
  int column;
  int row;
}Coord;

/* 
 * Neighborhood structure consists of the GLCM descriptors entropy and
 * energy for each of 2 distance and 4 angels 
 */
typedef struct {
  float entropy;
  float energy;
}Descriptors;

typedef struct {
  Descriptors deg0;
  Descriptors deg45;
  Descriptors deg90;
  Descriptors deg135;
}Angeles;

typedef struct {
  Angeles distShort;
  Angeles distLong;
}Neighborhood;

typedef short int Pixel; /* short int;*/


void drawLineSegment(Pixel *image,
		     Coord startPoint,
		     Coord endPoint,
		     int startValue,
		     int endValue,
		     int thickness,
		     int dimension)
  {
    int changeColumn, changeRow;
    int delta;
    int column, row;
    float value, valueDelta;
    int t;

    changeColumn = endPoint.column - startPoint.column;
    changeRow = endPoint.row - startPoint.row;
    
    assert((changeRow != 0) || (changeColumn != 0));

    column = startPoint.column;
    row = startPoint.row;
    value = startValue;

    if (ABS(changeColumn) > ABS(changeRow)){
      valueDelta = ((float) endValue - startValue)/
	            ((float) ABS(changeColumn));
      delta = 2*ABS(row) - ABS(column);
      for (column = startPoint.column;
	   column == endPoint.column+sign(changeColumn);
	   column += sign(changeColumn)){
	for (t = MAX(0, row-thickness/2);
	     t < MIN(dimension, row+thickness - thickness/2);
	     t++)
	  image[t*dimension + column] = (int)value;
	value += valueDelta;
	if (delta >= 0){
	  row += sign(changeRow);
	  delta -= 2*ABS(changeColumn);
	}
	column += sign(changeColumn);
	delta += 2*ABS(changeRow);
      }
    }
    else {
      valueDelta = ((float) endValue - startValue)/
	           ((float) ABS(changeRow));
      delta = 2* ABS(column) - ABS(row);
      for (row = startPoint.row;
	   row == endPoint.row + sign(changeRow);
	   row += sign(changeRow)){
	for (t = MAX(0, column-thickness/2);
	     t < MIN(dimension, row + thickness - thickness/2);
	     t++)
	  image[row*dimension + t] = (int)value;
	if (delta >= 0){
	  column += sign(changeColumn);
	    delta -= 2*ABS(changeRow);
	}
	row += sign(changeRow);
	delta += 2*ABS(changeColumn);
      }
    }
    return;
  }



Pixel *createImage (int dimension,
		    Pixel maxPixel,
		    int numberLines,
		    int minThickness,
		    int maxThickness)
  {
    Pixel *image;
    int i;
    
    Coord startPoint;
    Coord endPoint;
    int thickness;
    int startValue;
    int endValue;

    image = (Pixel *)malloc(sizeof(Pixel) * dimension * dimension);
    assert (image != NULL);
    for (i=0; i<dimension*dimension; i++){
      image[i] = 0;
    }

    for (i=0; i<numberLines; i++){
      float temp;
      float prev;

      temp = randomUInt(0, dimension*dimension - 1);
      startPoint.row = (int) temp/dimension;
      startPoint.column = (int) temp % dimension;
      prev = temp;

      while((temp = randomUInt(0, dimension*dimension -1)) == prev);

      endPoint.row = (int) temp/dimension;
      endPoint.column = (int) temp% dimension;

      thickness = randomUInt(minThickness, maxThickness);
      startValue = randomUInt(MIN_PIXEL, maxPixel);
      endValue = randomUInt(MIN_PIXEL, maxPixel);

      drawLineSegment(image, startPoint, endPoint,
		      startValue, endValue, thickness, dimension);
    }
    return(image);
  }


void calcEntropyEnergy(
		       int *sumHist,
		       int *diffHist,
		       Pixel *image,
		       int numBins,
		       int dx,
		       int dy,
		       float *entropy,
		       float *energy,
		       int dimension,
		        Pixel maxPixel)
  {
    int index;
    int totalNumPixels;
    int rowIndex;
    int rowLow, rowHigh;
    int columnIndex;
    int columnLow, columnHigh;
    int columnForPixelAtDistance;
    int rowForPixelAtDistance;
    int value0RowOffset;
    int value1RowOffset;

    *entropy = 0.0;
    *energy = 0.0;
    
    for (index = 0; index < numBins; index++){
      sumHist[index] = 0;
      diffHist[index] = 0;
    }
    
    if (dy < 0){
      rowLow = -dy;
      rowHigh = dimension;
    }
    else {
      rowLow = 0;
      rowHigh = dimension - dy;
    }
    if (dx < 0){
      columnLow = -dx;
      columnHigh = dimension;
    }
    else
      {
	columnLow = 0;
	columnHigh = dimension - dx;
      }

    totalNumPixels = 0;
    value0RowOffset = rowLow * dimension;
    value1RowOffset = (rowLow + dy) * dimension;

    for (rowIndex = rowLow; rowIndex<rowHigh; rowIndex++){
      for (columnIndex= columnLow; columnIndex<columnHigh;
	   columnIndex++){
	int value0;
	int value1;
	int binIndex;

	rowForPixelAtDistance = rowIndex + dy;
	columnForPixelAtDistance = columnIndex + dx;

	value0 = *(image + value0RowOffset + columnIndex);
	value1 = *(image + value1RowOffset + 
		   columnForPixelAtDistance);

	binIndex = value0 + value1 - 2*MIN_PIXEL;
	assert((binIndex >= 0) && (binIndex < numBins));
	sumHist[binIndex] += 1;
	binIndex = value0 - value1 + maxPixel - MIN_PIXEL;

	assert((binIndex >= 0) && (binIndex < numBins));

	diffHist[binIndex] += 1;	
	totalNumPixels += 1;

      }
      
      value0RowOffset += dimension;
      value1RowOffset += dimension;

    }
    

    if (totalNumPixels > 0){
      int index;
      double energySum;
      double energyDifference;
      double entropyValue;
      double sumNormalized;
      double diffNormalized;
      double scale;

      energySum = (double) 0;
      energyDifference = (double) 0;
      entropyValue = (double) 0;
      scale = 1.e0/totalNumPixels;
      for (index = 0; index<numBins; index++){
	if (sumHist[index] > 0){
	  sumNormalized = (double) sumHist[index]*scale;
	  entropyValue = entropyValue - sumNormalized * 
	    log((double)sumNormalized);
	  energySum = energySum + sumNormalized * sumNormalized ;
	}
	if (diffHist[index] > 0){
	  diffNormalized = (double)diffHist[index]*scale;
	  entropyValue = entropyValue - diffNormalized * log(diffNormalized);
	  energyDifference = energyDifference +
	    diffNormalized * diffNormalized;
	}
      }
      *energy = energySum * energyDifference;
      *entropy = entropyValue;
    }
    return;
  }

 
void neighborhoodCalculation
  (Pixel *image,
   int dimension,
   int distanceShort,
   int distanceLong,
   Neighborhood *neighborhood,
   Pixel maxPixel)
  {
    int *sumHist, *diffHist;
    int numBins;

    numBins = (2 * (maxPixel - MIN_PIXEL + 1) -1);
    sumHist = (int *) malloc(numBins * sizeof(int));
    assert (sumHist != NULL);
    diffHist = (int *)malloc(numBins * sizeof(int));
    assert(diffHist != NULL);
    
    printf(" before short calc deg0\n"); 
    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      distanceShort, 0,
		      &(neighborhood->distShort.deg0.entropy),
		      &(neighborhood->distShort.deg0.energy), dimension,
		      maxPixel);
  
    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      distanceShort, distanceShort,
		      &(neighborhood->distShort.deg45.entropy),
		      &(neighborhood->distShort.deg45.energy), dimension,
		      maxPixel);
   
    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      0, distanceShort,
		      &(neighborhood->distShort.deg90.entropy),
		      &(neighborhood->distShort.deg90.energy), dimension,
		      maxPixel);
   
    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      -distanceShort, distanceShort,
		      &(neighborhood->distShort.deg135.entropy),
		      &(neighborhood->distShort.deg135.energy), dimension,
		      maxPixel);

    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      distanceLong, 0,
		      &(neighborhood->distLong.deg0.entropy),
		      &(neighborhood->distLong.deg0.energy), dimension,
		      maxPixel);
  
    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      distanceLong, distanceLong,
		      &(neighborhood->distLong.deg45.entropy),
		      &(neighborhood->distLong.deg45.energy), dimension,
		      maxPixel);
      
    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      0, distanceLong,
		      &(neighborhood->distLong.deg90.entropy),
		      &(neighborhood->distLong.deg90.energy), dimension,
		      maxPixel);

    calcEntropyEnergy(sumHist, diffHist, image, numBins,
		      -distanceLong, distanceLong,
		      &(neighborhood->distLong.deg135.entropy),
		      &(neighborhood->distLong.deg135.energy), dimension,
		      maxPixel);

    free(sumHist);
    free(diffHist);
    
    return;
  }

