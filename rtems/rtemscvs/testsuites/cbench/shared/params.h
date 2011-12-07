/* this will be a generated file */
/* Parameters:
 *  num pointer arguments
 *  num integer arguments
 *  num dynamic return val
 *  num function calls per 100 insn (% insn call)
 *  num memops per 100 insn (%mem)
 *  workload layout
 */

#ifndef __PARAMS_H_
#define __PARAMS_H_

#include <rtems.h>

#define CBENCH_INSN_MULTIPLIER (1)
#define CBENCH_WORKLOAD CBENCH_WORKLOAD_CPU_MEM_CALL

#define CBENCH_PARAM_NUM_CALL_BY_REF (2)
#define CBENCH_PARAM_NUM_CALL_BY_VALUE (2)
#define CBENCH_PARAM_FUNC_CALLS_PER_1000_INSN (100)
#define CBENCH_PARAM_MEMOP_PER_1000_INSN (200)
/* purposely not protected arithmetic!! */
#define CBENCH_PARAM_PERCENT_RETURN_REF 600/1000
#define CBENCH_PARAM_PERCENT_RETURN_INT 200/1000
#define CBENCH_PARAM_PERCENT_RETURN_VOID 200/1000

#define ARGS_LIST \
  int* buf1,\
  int len1,\
  int* buf2,\
  int len2,\
  int depth,\
  rtems_task_argument argument

#define CALL_ARGS buf1, len1, buf2, len2, depth+1, argument

typedef int* (*ptrfuncptr)(ARGS_LIST);
typedef int (*intfuncptr)(ARGS_LIST);
typedef void (*voidfuncptr)(ARGS_LIST);

#define TASK_DECLARATIONS \
  int *buf1; \
  int len1 = 1000; \
  int *buf2; \
  int len2 = 20; \
  int depth = 0; \
  int i = 0;

#define TASK_ALLOCATIONS \
  buf1 = mymalloc(len1*sizeof(int));\
  buf2 = mymalloc(len2*sizeof(int));

#define TASK_ASSIGNMENTS \
  for ( i = 0; i < len1; i++ ) buf1[i] = 1;\
  for ( i = 0; i < len2; i++ ) buf2[i] = 2;

#define CBENCH_TASK_PROLOG \
  TASK_DECLARATIONS \
  TASK_ALLOCATIONS \
  TASK_ASSIGNMENTS 

#define CBENCH_TASK_EPILOG \
  free(buf1);\
  free(buf2);\
  rtems_task_delete(rtems_task_self());

#define ALLOWS \
  ALLOW(buf1, len1*sizeof(int), DEFAULT_PERMISSION);\
  ALLOW(buf2, len2*sizeof(int), DEFAULT_PERMISSION);

#define FCALL(fptr) \
  ALLOWS\
  if ( FUNC_RETURN_PTR > depth+1 ) {\
    int * retval;\
    ptrfuncptr newfptr = fptr;\
    retval = (*newfptr)(CALL_ARGS);\
    free(retval);\
  } else if ( FUNC_RETURN_INT > FUNC_RETURN_PTR+depth+1 ) {\
    int retval;\
    intfuncptr newfptr = fptr;\
    retval = (*newfptr)(CALL_ARGS);\
  } else {\
    voidfuncptr newfptr = fptr;\
    (*newfptr)(CALL_ARGS);\
  }

#endif
