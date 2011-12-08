/* This is a generated file. DO NOT EDIT. */

/* command line: -f 20  */
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
#if (6 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00006;
#elif (6 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00006;
#else
  voidfuncptr f = &cbench_func_00006;
#endif

CBENCH_WORKLOAD;

#if (5 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (5 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (6 <= FUNC_RETURN_PTR)
int *
#elif (6 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00006(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (7 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00007;
#elif (7 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00007;
#else
  voidfuncptr f = &cbench_func_00007;
#endif

CBENCH_WORKLOAD;

#if (6 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (6 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (7 <= FUNC_RETURN_PTR)
int *
#elif (7 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00007(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (8 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00008;
#elif (8 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00008;
#else
  voidfuncptr f = &cbench_func_00008;
#endif

CBENCH_WORKLOAD;

#if (7 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (7 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (8 <= FUNC_RETURN_PTR)
int *
#elif (8 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00008(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (9 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00009;
#elif (9 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00009;
#else
  voidfuncptr f = &cbench_func_00009;
#endif

CBENCH_WORKLOAD;

#if (8 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (8 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (9 <= FUNC_RETURN_PTR)
int *
#elif (9 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00009(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (10 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00010;
#elif (10 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00010;
#else
  voidfuncptr f = &cbench_func_00010;
#endif

CBENCH_WORKLOAD;

#if (9 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (9 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (10 <= FUNC_RETURN_PTR)
int *
#elif (10 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00010(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (11 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00011;
#elif (11 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00011;
#else
  voidfuncptr f = &cbench_func_00011;
#endif

CBENCH_WORKLOAD;

#if (10 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (10 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (11 <= FUNC_RETURN_PTR)
int *
#elif (11 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00011(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (12 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00012;
#elif (12 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00012;
#else
  voidfuncptr f = &cbench_func_00012;
#endif

CBENCH_WORKLOAD;

#if (11 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (11 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (12 <= FUNC_RETURN_PTR)
int *
#elif (12 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00012(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (13 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00013;
#elif (13 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00013;
#else
  voidfuncptr f = &cbench_func_00013;
#endif

CBENCH_WORKLOAD;

#if (12 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (12 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (13 <= FUNC_RETURN_PTR)
int *
#elif (13 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00013(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (14 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00014;
#elif (14 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00014;
#else
  voidfuncptr f = &cbench_func_00014;
#endif

CBENCH_WORKLOAD;

#if (13 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (13 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (14 <= FUNC_RETURN_PTR)
int *
#elif (14 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00014(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (15 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00015;
#elif (15 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00015;
#else
  voidfuncptr f = &cbench_func_00015;
#endif

CBENCH_WORKLOAD;

#if (14 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (14 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (15 <= FUNC_RETURN_PTR)
int *
#elif (15 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00015(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (16 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00016;
#elif (16 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00016;
#else
  voidfuncptr f = &cbench_func_00016;
#endif

CBENCH_WORKLOAD;

#if (15 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (15 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (16 <= FUNC_RETURN_PTR)
int *
#elif (16 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00016(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (17 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00017;
#elif (17 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00017;
#else
  voidfuncptr f = &cbench_func_00017;
#endif

CBENCH_WORKLOAD;

#if (16 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (16 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (17 <= FUNC_RETURN_PTR)
int *
#elif (17 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00017(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (18 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00018;
#elif (18 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00018;
#else
  voidfuncptr f = &cbench_func_00018;
#endif

CBENCH_WORKLOAD;

#if (17 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (17 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (18 <= FUNC_RETURN_PTR)
int *
#elif (18 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00018(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (19 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00019;
#elif (19 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00019;
#else
  voidfuncptr f = &cbench_func_00019;
#endif

CBENCH_WORKLOAD;

#if (18 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (18 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (19 <= FUNC_RETURN_PTR)
int *
#elif (19 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00019(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (20 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00020;
#elif (20 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00020;
#else
  voidfuncptr f = &cbench_func_00020;
#endif

CBENCH_WORKLOAD;

#if (19 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (19 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (20 <= FUNC_RETURN_PTR)
int *
#elif (20 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00020(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_vleaf;
CBENCH_WORKLOAD;

#if (20 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (20 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

