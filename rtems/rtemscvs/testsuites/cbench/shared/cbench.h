#ifndef _CBENCH_H_
#define _CBENCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtems/rtems/types.h"

/* cbench interface */
extern void cbench_initialize( void );
extern void cbench_warmup( void );
extern void cbench_work( void );

#ifdef __cplusplus
}
#endif

#endif
