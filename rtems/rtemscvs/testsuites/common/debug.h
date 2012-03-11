#ifndef __DEBUG_H_
#define __DEBUG_H_

////////////////////////////////////////#undef DEBUG
#undef DEBUG


#if defined(DEBUG)
#include <stdio.h>

#define DPRINTF(...) \
      printf(__VA_ARGS__)

#define DPRINTK(...) \
      printk(__VA_ARGS__)


#define DPUTS(str) puts((str))
#else
#define DPRINTF(...)
#define DPRINTK(...)
#define DPUTS(str)
#endif

#endif
