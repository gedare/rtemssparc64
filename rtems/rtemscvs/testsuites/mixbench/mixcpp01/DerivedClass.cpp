#ifdef __cplusplus 
extern "C" {
	#include "system.h"
}
#endif

using namespace std;
class BaseClass {

private:
  	int i;
protected: 
  	int j;
public:
  void set(int a, int b);
  int show();
};

void BaseClass::set(int a, int b) { 
     i = a; 
     j = b; 
  }

int BaseClass::show() { 
	  return i+j; 
  }


class DerivedClass : public BaseClass {
  int k;
public:
  DerivedClass(int x) ;
  int show();
  int showk();
};

DerivedClass::DerivedClass(int x) { 
   k = x; 
}
int DerivedClass::show(){
  j=5;
  return k;
}
int DerivedClass::showk() { 
   k = 100;
   j = 4;
   return k;
}


#ifdef __cplusplus 
extern "C" {
	int testAnotherClass(int x){
		DerivedClass *ob = new DerivedClass(3);

		ob->set(1, 2); // access member of BaseClass
		ob->show();    // access member of BaseClass
		ob->showk();   // uses member of DerivedClass class
		return x+ob->showk();
	}
}
#endif

