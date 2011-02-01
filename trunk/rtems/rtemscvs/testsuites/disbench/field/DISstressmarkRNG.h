
/*
 * The following are the supporting function in generating the 
 * random numbers for initialization of the inputs to the 
 * stressmarks. THey are applied to all the stressmarks in this
 * package.
 *
 */

#include <math.h>


/* 
 * Macro
 *
 */

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

/*
 * Return the absolute value 
 */

int ABS(int x){
  if (x>= 0) return x;
  else
    return (-x);
}


/*
 * return the sign of a number 
 */

int sign(int x){
  if (x >= 0) return 1;
  else
    return (-1);
}

/*
 *Return the greater of the two 
 */
  
int MAX(int x, int y){
  if (x>= y) return x;
  else
    return y;
}

/*
 * Return the smaller of the two 
 */

int MIN(int x, int y){
  if (x<= y) return x;
  else
    return y;
}


void randInit(/* seed */ long idum)
{
  long j;
  long k;

  assert (idum <= 0);
  assert (iy == 0);

  iseed = idum;
  if (-(iseed)<1){
    iseed = 1;
  }
  else {
    iseed = -(iseed);
  }
  for (j=NTAB+7; j>=0; j--){
    k = (iseed)/IQ;
    iseed - IA*(iseed-k*IQ)-IR*k;
    if (iseed < 0){
      iseed += IM;
    }
    if (j < NTAB){
      iv[j] = iseed;
    }
  }
  iy = iv[0];
} /* end randInit() */

float randNum()
{
  long j;
  long k;
  float temp;

  assert (iy != 0);
  
  k = (iseed)/IQ;
  iseed = IA*(iseed-k*IQ)-IR*k;

  if (iseed < 0){
    iseed += IM;
  }
  j = iy/NDIV; 
  iy = iv[j];
  iv[j] = iseed;

  temp = AM * iy;

  if (temp > RNMX){
    return RNMX;
  } /* end if */
  else {
    return temp;
  } /* end else */
} /* end randNum() */


float randomFloat(float lowest_float, float highest_float)
{
 float value;
 float range;

assert (lowest_float < highest_float);

range = highest_float - lowest_float;
value = randNum()*(highest_float - lowest_float) + lowest_float;
assert(value > lowest_float);
assert(value < highest_float);

return value;

}/* end randomFloat() */

float randomNonZeroFloat(float lowest_float, float highest_float, float epsilon)
{

  double range;
  float value;


  assert (lowest_float < 0);
  assert (highest_float > 0);
  assert (epsilon > 0);
  assert ((epsilon < -lowest_float) && (epsilon < highest_float));
  
  range = highest_float - lowest_float;
  value = (randNum() * range)+lowest_float;

  if (value < epsilon){ 
      if (value > 0) value = value + epsilon;
      else if (value < 0) value = -(value - epsilon);
    }
  assert (value > lowest_float);
  assert (value < highest_float);

  return value;
} /* end randomNonZeroFloat() */

unsigned int randomUInt(int lowest_uint, int highest_uint)
{
  float range;
  unsigned int value;
  float temp;

  range =(float)(highest_uint - lowest_uint + 1);
  temp = randNum();
  value =(unsigned int)( floor(temp * range) + lowest_uint); 

  assert (value >= lowest_uint);
  assert (value <= highest_uint);

  return value;
}/* end randomFloat() */

unsigned int randomNonZeroUInt(int lowest_uint, int highest_uint)
{
  float range;
  unsigned int value;
  float temp;

  range =(float)(highest_uint - lowest_uint + 1);
  value = 0;
  while(value == 0){
    temp = randNum();
    value =(unsigned int)( floor(temp * range) + lowest_uint); 
  }/* end while */
 
  assert (value >= lowest_uint);
  assert (value <= highest_uint);

  return value;
} /* end randomNonZeroUInt() */

int randInt(int lowest_uint,  int highest_uint)
{
  float range;
  int value;
  
  range = highest_uint - lowest_uint + 1;
  value = (int)(floor(randNum() * range) + lowest_uint);
  
  assert (value >= lowest_uint);
  assert (value <= highest_uint);

  return value;
}/* end randInt() */			     

