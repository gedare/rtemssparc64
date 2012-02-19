#ifdef __cplusplus 
extern "C" {
	#include "system.h"
}
#endif

using namespace std;

class SimpleClass {
  int len;
public:
  SimpleClass(int l);
  int getLen();
  void print();
};

SimpleClass::SimpleClass(int l)
{
  len = l;

  int i;
  int a = 0;
  for(i = 0; i <len; i++) 
      a+=i ;
  len += a;
}

int SimpleClass::getLen()
{
	return this->len;
}

void SimpleClass::print()
{
	//printf("%d",this->len);
	len=456; 
}


#ifdef __cplusplus 
extern "C" {
	int testSimpleClass()
	{
	  SimpleClass l(10);
	  l.print();
	  int len = l.getLen();
	  return len;
	}
}
#endif


