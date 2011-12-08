/* This is a generated file. DO NOT EDIT. */

/* command line: -f 5  */
#include "generated.h"

int* cbench_func_pleaf(ARGS_LIST) {
  CBENCH_PROLOG;
  __asm__ __volatile__("nop\n\t");
  CBENCH_EPILOG_RET_PTR;
}

int cbench_func_ileaf(ARGS_LIST) {
  CBENCH_PROLOG;
  __asm__ __volatile__("nop\n\t");
  CBENCH_EPILOG_RET_INT;
}

void cbench_func_vleaf(ARGS_LIST) {
  CBENCH_PROLOG;
  __asm__ __volatile__("nop\n\t");
  CBENCH_EPILOG_VOID;
}


#if (1 <= FUNC_RETURN_PTR)
int *
#elif (1 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00001(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (2 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00002;
#elif (2 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00002;
#else
  voidfuncptr f = &cbench_func_00002;
#endif

CBENCH_WORKLOAD;

#if (1 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (1 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (2 <= FUNC_RETURN_PTR)
int *
#elif (2 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00002(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (3 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00003;
#elif (3 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00003;
#else
  voidfuncptr f = &cbench_func_00003;
#endif

CBENCH_WORKLOAD;

#if (2 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (2 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (3 <= FUNC_RETURN_PTR)
int *
#elif (3 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00003(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (4 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00004;
#elif (4 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00004;
#else
  voidfuncptr f = &cbench_func_00004;
#endif

CBENCH_WORKLOAD;

#if (3 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (3 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (4 <= FUNC_RETURN_PTR)
int *
#elif (4 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00004(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (5 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00005;
#elif (5 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00005;
#else
  voidfuncptr f = &cbench_func_00005;
#endif

CBENCH_WORKLOAD;

#if (4 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (4 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (5 <= FUNC_RETURN_PTR)
int *
#elif (5 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00005(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_vleaf;
CBENCH_WORKLOAD;

#if (5 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (5 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

