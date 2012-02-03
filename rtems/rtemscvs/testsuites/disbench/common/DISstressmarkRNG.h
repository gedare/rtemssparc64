#ifndef __DISStressmarkRNG_h
#define __DISStressmarkRNG_h

#include <math.h>

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

static long iy=0;
static long iv[NTAB];
static long iseed;

int ABS(int x);

int sign(int x);
int MAX(int x, int y);

int MIN(int x, int y);

void randInit(long idum);

float randNum();


float randomFloat(float lowest_float, float highest_float);
float randomNonZeroFloat(float lowest_float, float highest_float, float epsilon);

unsigned int randomUInt(int lowest_uint, int highest_uint);
unsigned int randomNonZeroUInt(int lowest_uint, int highest_uint);

int randInt(int lowest_uint,  int highest_uint);


#endif
