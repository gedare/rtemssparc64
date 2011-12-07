/* This will be an automatically generated file */
#ifndef __GENERATED_H_
#define __GENERATED_H_

#define NUM_TASKS (2)
#define NUM_FUNCTIONS (5)

#define FUNC_RETURN_PTR (NUM_FUNCTIONS*CBENCH_PARAM_PERCENT_RETURN_REF)
#define FUNC_RETURN_INT (NUM_FUNCTIONS*CBENCH_PARAM_PERCENT_RETURN_INT)
#define FUNC_RETURN_VOID (NUM_FUNCTIONS*CBENCH_PARAM_PERCENT_RETURN_VOID)

#include "../shared/params.h"
#include "../shared/cbench.h"

int* cbench_func_pleaf(ARGS_LIST);
int cbench_func_ileaf(ARGS_LIST);
void cbench_func_vleaf(ARGS_LIST);

#if (1 <= FUNC_RETURN_PTR)
int *
#elif (1 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0001(ARGS_LIST);

#if (2 <= FUNC_RETURN_PTR)
int *
#elif (2 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0002(ARGS_LIST);

#if (3 <= FUNC_RETURN_PTR)
int *
#elif (3 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0003(ARGS_LIST);

#if (4 <= FUNC_RETURN_PTR)
int *
#elif (4 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0004(ARGS_LIST);

#if (5 <= FUNC_RETURN_PTR)
int *
#elif (5 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_0005(ARGS_LIST);

#endif
