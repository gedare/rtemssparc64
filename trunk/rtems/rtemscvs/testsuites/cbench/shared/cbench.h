#ifndef _CBENCH_H_
#define _CBENCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems.h>
#include <allow.h>

/* cbench interface */
extern void cbench_task_entry(rtems_task_argument argument);
extern void cbench_initialize( void );
extern void cbench_warmup( void );
extern void cbench_work( void );

#ifdef __cplusplus
}
#endif

#endif
