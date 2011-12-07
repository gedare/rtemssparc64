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

  CBENCH_EPILOG_RET_PTR;
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
  ptrfuncptr f = &cbench_func_00003;

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
cbench_func_00003(ARGS_LIST)
{
  CBENCH_PROLOG;
  intfuncptr f = &cbench_func_00004;

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
cbench_func_00004(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_00005;

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
cbench_func_00005(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_vleaf;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_VOID;
}

