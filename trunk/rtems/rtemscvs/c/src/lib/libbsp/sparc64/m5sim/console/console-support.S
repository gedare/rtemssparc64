
#include "syscallasm.h"

#define concat(a,b)	a##b
#define concat3(a,b,c)	a##b##c
#define makesys(a)	concat (SYS_, a)
#define make_r_fn(a)	concat3 (_, a, _r)

#ifdef REENT
	defsyscall_r (make_r_fn (mkbsp_write), makesys (write))
#else
	defsyscall (m5bsp_write, makesys (write))
#endif
