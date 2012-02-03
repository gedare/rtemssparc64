#include "DISstressmarkRNG.h"
#include <assert.h>

int ABS(int x){
  if (x>= 0) return x;
  else
    return (-x);
}

int sign(int x){
  if (x >= 0) return 1;
  else
    return (-1);
}

int MAX(int x, int y){
  if (x>= y) return x;
  else
    return y;
}

int MIN(int x, int y){
  if (x<= y) return x;
  else
    return y;
}

void randInit(long idum)
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
}

float randNum()
{
  long j;
  long k;
  float temp;

  assert (iy != 0);

  //printf(" iseed is %d \n", iseed);
  k = (iseed)/IQ;
  iseed = IA*(iseed-k*IQ)-IR*k;

  if (iseed < 0){
    iseed += IM;
  }
  j = iy/NDIV;
  iy = iv[j];
  iv[j] = iseed;

  temp = AM * iy;
  // printf(" temp is %d\n", temp);

  if (temp > RNMX){
    return RNMX;
  }
  else {
    return temp;
  }
}


float randomFloat(float lowest_float, float highest_float)
{
 float value;
 float range;

assert (lowest_float < highest_float);

range = highest_float - lowest_float;
value = randNum()*(highest_float - lowest_float) + lowest_float;
assert(value >= lowest_float);
assert(value <= highest_float);

return value;

}

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
  //printf(" value is %f\n", value);

  if (ABS(value) < epsilon)
    {
      if (value > 0) value = value + epsilon;
      else if (value < 0) value = value - epsilon;

    }

  // printf(" value is %f \n", value);
  assert (value >= lowest_float);
  assert (value <= highest_float);

  return value;
}

unsigned int randomUInt(int lowest_uint, int highest_uint)
{
  float range;
  unsigned int value;
  float temp;

  range =(float)(highest_uint - lowest_uint + 1);
  temp = randNum();
  value =(unsigned int)( floor(temp * range) + lowest_uint);

  //printf("value is %u\n",value);

  assert (value >= lowest_uint);
  assert (value <= highest_uint);

  return value;
}

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
  }
  //printf("value is %u\n",value);

  assert (value >= lowest_uint);
  assert (value <= highest_uint);

  return value;
}

int randInt(int lowest_uint,  int highest_uint)
{
  float range;
  int value;

  range = highest_uint - lowest_uint + 1;
  value = (int)(floor(randNum() * range) + lowest_uint);

  assert (value >= lowest_uint);
  assert (value <= highest_uint);

  return value;
}
