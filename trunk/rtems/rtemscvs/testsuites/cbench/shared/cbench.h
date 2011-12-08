#ifndef _CBENCH_H_
#define _CBENCH_H_

#ifdef __cplusplus
extern "C" {
#endif

//#define CBENCH_DEBUG

#if defined(CBENCH_DEBUG)
#include <stdio.h>
#define DPRINTF(...) \
      printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

#include <rtems.h>
#include <allow.h>

/* cbench interface */
extern void cbench_task_entry(rtems_task_argument argument);
extern void cbench_initialize( void );
extern void cbench_warmup( void );
extern void cbench_work( void );

/* some fixed parameters and helper macros/functions */
#define DEFAULT_PERMISSION (3LL)
#define DECLARATIONS \
  int i = 0;\
  int *p;

#define CBENCH_PROLOG \
  DECLARATIONS

#define CBENCH_EPILOG_RET_PTR \
  p = mymalloc(sizeof(int));\
  *p = 42;\
  ALLOW(p, sizeof(int), DEFAULT_PERMISSION);\
  return p;

#define CBENCH_EPILOG_RET_INT \
  return i;

#define CBENCH_EPILOG_VOID \
  return;

#define CBENCH_WORKLOAD_INTERLEAVED \
  p = buf1;\
  for ( i = 0; i < 1000 * CBENCH_INSN_MULTIPLIER; i++ ) {\
    if ( i % (1000*CBENCH_INSN_MULTIPLIER/CBENCH_PARAM_MEMOP_PER_1000_INSN) == 0 ) {\
      *p++ = i;\
      if ( p >= buf1 + len1 ) p = buf1;\
    }\
    if ( i == 0 ) { \
      FCALL(f);\
      f = cbench_func_leaf;\
      depth = NUM_FUNCTIONS;\
    } else if ( i % (CBENCH_INSN_MULTIPLIER*1000/CBENCH_PARAM_FUNC_CALLS_PER_1000_INSN) == 0 ) { \
      FCALL(f);\
    }\
  }\

#define MEM_WORKLOAD \
  p = buf1;\
  for ( i = 0; \
      i < CBENCH_INSN_MULTIPLIER * CBENCH_PARAM_MEMOP_PER_1000_INSN;\
      i++ ) {\
    *p++ = i;\
    if ( p >= buf1 + len1 ) p = buf1;\
  }\

#define CPU_WORKLOAD \
  __asm__ __volatile__ (\
      "or %%g0, %0, %%l0\n\t"\
      "0: dec %%l0\n\t"\
      "   brgz %%l0, 0b\n\t"\
      "   nop\n\t"\
      : : "n" (1000*CBENCH_INSN_MULTIPLIER) : "l0" );

#define CALL_WORKLOAD \
  FCALL(f);\
  if ( FUNC_RETURN_PTR > depth+1 ) {\
    f = &cbench_func_pleaf;\
  } else if ( FUNC_RETURN_INT > FUNC_RETURN_PTR+depth+1 ) {\
    f = &cbench_func_ileaf;\
  } else {\
    f = &cbench_func_vleaf;\
  }\
  for ( i = 0;\
      i < (CBENCH_INSN_MULTIPLIER * CBENCH_PARAM_FUNC_CALLS_PER_1000_INSN);\
      i++ ) { \
    FCALL(f);\
  }\

#define CBENCH_WORKLOAD_CPU_MEM_CALL \
  CPU_WORKLOAD \
  MEM_WORKLOAD \
  CALL_WORKLOAD \


#define CBENCH_WORKLOAD_CPU_CALL_MEM \
  CPU_WORKLOAD \
  CALL_WORKLOAD \
  MEM_WORKLOAD \


#define CBENCH_WORKLOAD_MEM_CPU_CALL \
  MEM_WORKLOAD \
  CPU_WORKLOAD \
  CALL_WORKLOAD \


#define CBENCH_WORKLOAD_MEM_CALL_CPU \
  MEM_WORKLOAD \
  CALL_WORKLOAD \
  CPU_WORKLOAD \


#define CBENCH_WORKLOAD_CALL_CPU_MEM \
  CALL_WORKLOAD \
  CPU_WORKLOAD \
  MEM_WORKLOAD \


#define CBENCH_WORKLOAD_CALL_MEM_CPU \
  CALL_WORKLOAD \
  MEM_WORKLOAD \
  CPU_WORKLOAD \

#ifdef __cplusplus
}
#endif

#endif
