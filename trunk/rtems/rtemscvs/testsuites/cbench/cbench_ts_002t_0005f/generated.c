/* This will be an automatically generated file */
/*
 * GENERATED FILE (or it will be...)
 */

#include "generated.h"

int* cbench_func_001(ARGS_LIST)
{
  CBENCH_PROLOG;
  ptrfuncptr f = &cbench_func_002;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_PTR;
}
int* cbench_func_002(ARGS_LIST)
{
  CBENCH_PROLOG;
  ptrfuncptr f = &cbench_func_003;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_PTR;
}
int* cbench_func_003(ARGS_LIST)
{
  CBENCH_PROLOG;
  intfuncptr f = &cbench_func_004;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_PTR;
}
int cbench_func_004(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_005;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_RET_INT;
}
void cbench_func_005(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = NULL;

  CBENCH_WORKLOAD;

  CBENCH_EPILOG_VOID;
}
