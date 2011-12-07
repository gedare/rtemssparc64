/* This will be an automatically generated file */
/*
 * GENERATED FILE (or it will be...)
 */

#include "generated.h"

#if (1 <= FUNC_RETURN_PTR)
int *
#elif (1 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0001(ARGS_LIST)
{
  CBENCH_PROLOG;
  ptrfuncptr f = &cbench_func_0002;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_PTR;
}

#if (2 <= FUNC_RETURN_PTR)
int *
#elif (2 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0002(ARGS_LIST)
{
  CBENCH_PROLOG;
  ptrfuncptr f = &cbench_func_0003;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_PTR;
}

#if (3 <= FUNC_RETURN_PTR)
int *
#elif (3 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0003(ARGS_LIST)
{
  CBENCH_PROLOG;
  intfuncptr f = &cbench_func_0004;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_PTR;
}

#if (4 <= FUNC_RETURN_PTR)
int *
#elif (4 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0004(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_0005;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_INT;
}

#if (5 <= FUNC_RETURN_PTR)
int *
#elif (5 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0005(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_vleaf;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_VOID;
}

