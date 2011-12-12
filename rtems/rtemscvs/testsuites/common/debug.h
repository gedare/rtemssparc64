#ifndef __DEBUG_H_
#define __DEBUG_H_

//#define DEBUG
//#undef DEBUG

#if defined(DEBUG)
#include <stdio.h>
#endif

#if defined(DEBUG)
static inline void DPRINTF(char *s, ...) {
  printf(s, __VA_ARGS__);
}
static inline void DPRINTK(char *s, ...) {
  printk(s, __VA_ARGS__);
}
#define DPUTS(str) puts((str))
#else
#define DPRINTF(...)
#define DPRINTK(...)
#define DPUTS(str)
#endif

#endif
