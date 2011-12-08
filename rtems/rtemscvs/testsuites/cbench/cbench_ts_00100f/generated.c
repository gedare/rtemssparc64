/* This is a generated file. DO NOT EDIT. */

/* command line: -f 100  */
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
#if (21 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00021;
#elif (21 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00021;
#else
  voidfuncptr f = &cbench_func_00021;
#endif

CBENCH_WORKLOAD;

#if (20 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (20 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (21 <= FUNC_RETURN_PTR)
int *
#elif (21 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00021(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (22 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00022;
#elif (22 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00022;
#else
  voidfuncptr f = &cbench_func_00022;
#endif

CBENCH_WORKLOAD;

#if (21 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (21 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (22 <= FUNC_RETURN_PTR)
int *
#elif (22 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00022(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (23 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00023;
#elif (23 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00023;
#else
  voidfuncptr f = &cbench_func_00023;
#endif

CBENCH_WORKLOAD;

#if (22 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (22 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (23 <= FUNC_RETURN_PTR)
int *
#elif (23 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00023(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (24 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00024;
#elif (24 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00024;
#else
  voidfuncptr f = &cbench_func_00024;
#endif

CBENCH_WORKLOAD;

#if (23 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (23 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (24 <= FUNC_RETURN_PTR)
int *
#elif (24 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00024(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (25 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00025;
#elif (25 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00025;
#else
  voidfuncptr f = &cbench_func_00025;
#endif

CBENCH_WORKLOAD;

#if (24 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (24 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (25 <= FUNC_RETURN_PTR)
int *
#elif (25 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00025(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (26 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00026;
#elif (26 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00026;
#else
  voidfuncptr f = &cbench_func_00026;
#endif

CBENCH_WORKLOAD;

#if (25 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (25 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (26 <= FUNC_RETURN_PTR)
int *
#elif (26 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00026(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (27 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00027;
#elif (27 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00027;
#else
  voidfuncptr f = &cbench_func_00027;
#endif

CBENCH_WORKLOAD;

#if (26 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (26 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (27 <= FUNC_RETURN_PTR)
int *
#elif (27 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00027(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (28 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00028;
#elif (28 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00028;
#else
  voidfuncptr f = &cbench_func_00028;
#endif

CBENCH_WORKLOAD;

#if (27 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (27 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (28 <= FUNC_RETURN_PTR)
int *
#elif (28 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00028(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (29 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00029;
#elif (29 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00029;
#else
  voidfuncptr f = &cbench_func_00029;
#endif

CBENCH_WORKLOAD;

#if (28 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (28 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (29 <= FUNC_RETURN_PTR)
int *
#elif (29 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00029(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (30 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00030;
#elif (30 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00030;
#else
  voidfuncptr f = &cbench_func_00030;
#endif

CBENCH_WORKLOAD;

#if (29 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (29 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (30 <= FUNC_RETURN_PTR)
int *
#elif (30 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00030(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (31 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00031;
#elif (31 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00031;
#else
  voidfuncptr f = &cbench_func_00031;
#endif

CBENCH_WORKLOAD;

#if (30 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (30 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (31 <= FUNC_RETURN_PTR)
int *
#elif (31 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00031(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (32 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00032;
#elif (32 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00032;
#else
  voidfuncptr f = &cbench_func_00032;
#endif

CBENCH_WORKLOAD;

#if (31 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (31 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (32 <= FUNC_RETURN_PTR)
int *
#elif (32 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00032(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (33 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00033;
#elif (33 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00033;
#else
  voidfuncptr f = &cbench_func_00033;
#endif

CBENCH_WORKLOAD;

#if (32 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (32 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (33 <= FUNC_RETURN_PTR)
int *
#elif (33 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00033(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (34 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00034;
#elif (34 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00034;
#else
  voidfuncptr f = &cbench_func_00034;
#endif

CBENCH_WORKLOAD;

#if (33 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (33 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (34 <= FUNC_RETURN_PTR)
int *
#elif (34 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00034(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (35 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00035;
#elif (35 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00035;
#else
  voidfuncptr f = &cbench_func_00035;
#endif

CBENCH_WORKLOAD;

#if (34 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (34 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (35 <= FUNC_RETURN_PTR)
int *
#elif (35 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00035(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (36 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00036;
#elif (36 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00036;
#else
  voidfuncptr f = &cbench_func_00036;
#endif

CBENCH_WORKLOAD;

#if (35 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (35 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (36 <= FUNC_RETURN_PTR)
int *
#elif (36 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00036(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (37 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00037;
#elif (37 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00037;
#else
  voidfuncptr f = &cbench_func_00037;
#endif

CBENCH_WORKLOAD;

#if (36 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (36 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (37 <= FUNC_RETURN_PTR)
int *
#elif (37 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00037(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (38 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00038;
#elif (38 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00038;
#else
  voidfuncptr f = &cbench_func_00038;
#endif

CBENCH_WORKLOAD;

#if (37 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (37 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (38 <= FUNC_RETURN_PTR)
int *
#elif (38 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00038(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (39 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00039;
#elif (39 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00039;
#else
  voidfuncptr f = &cbench_func_00039;
#endif

CBENCH_WORKLOAD;

#if (38 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (38 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (39 <= FUNC_RETURN_PTR)
int *
#elif (39 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00039(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (40 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00040;
#elif (40 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00040;
#else
  voidfuncptr f = &cbench_func_00040;
#endif

CBENCH_WORKLOAD;

#if (39 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (39 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (40 <= FUNC_RETURN_PTR)
int *
#elif (40 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00040(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (41 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00041;
#elif (41 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00041;
#else
  voidfuncptr f = &cbench_func_00041;
#endif

CBENCH_WORKLOAD;

#if (40 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (40 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (41 <= FUNC_RETURN_PTR)
int *
#elif (41 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00041(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (42 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00042;
#elif (42 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00042;
#else
  voidfuncptr f = &cbench_func_00042;
#endif

CBENCH_WORKLOAD;

#if (41 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (41 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (42 <= FUNC_RETURN_PTR)
int *
#elif (42 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00042(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (43 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00043;
#elif (43 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00043;
#else
  voidfuncptr f = &cbench_func_00043;
#endif

CBENCH_WORKLOAD;

#if (42 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (42 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (43 <= FUNC_RETURN_PTR)
int *
#elif (43 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00043(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (44 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00044;
#elif (44 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00044;
#else
  voidfuncptr f = &cbench_func_00044;
#endif

CBENCH_WORKLOAD;

#if (43 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (43 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (44 <= FUNC_RETURN_PTR)
int *
#elif (44 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00044(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (45 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00045;
#elif (45 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00045;
#else
  voidfuncptr f = &cbench_func_00045;
#endif

CBENCH_WORKLOAD;

#if (44 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (44 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (45 <= FUNC_RETURN_PTR)
int *
#elif (45 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00045(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (46 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00046;
#elif (46 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00046;
#else
  voidfuncptr f = &cbench_func_00046;
#endif

CBENCH_WORKLOAD;

#if (45 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (45 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (46 <= FUNC_RETURN_PTR)
int *
#elif (46 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00046(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (47 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00047;
#elif (47 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00047;
#else
  voidfuncptr f = &cbench_func_00047;
#endif

CBENCH_WORKLOAD;

#if (46 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (46 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (47 <= FUNC_RETURN_PTR)
int *
#elif (47 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00047(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (48 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00048;
#elif (48 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00048;
#else
  voidfuncptr f = &cbench_func_00048;
#endif

CBENCH_WORKLOAD;

#if (47 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (47 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (48 <= FUNC_RETURN_PTR)
int *
#elif (48 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00048(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (49 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00049;
#elif (49 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00049;
#else
  voidfuncptr f = &cbench_func_00049;
#endif

CBENCH_WORKLOAD;

#if (48 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (48 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (49 <= FUNC_RETURN_PTR)
int *
#elif (49 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00049(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (50 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00050;
#elif (50 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00050;
#else
  voidfuncptr f = &cbench_func_00050;
#endif

CBENCH_WORKLOAD;

#if (49 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (49 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (50 <= FUNC_RETURN_PTR)
int *
#elif (50 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00050(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (51 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00051;
#elif (51 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00051;
#else
  voidfuncptr f = &cbench_func_00051;
#endif

CBENCH_WORKLOAD;

#if (50 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (50 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (51 <= FUNC_RETURN_PTR)
int *
#elif (51 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00051(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (52 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00052;
#elif (52 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00052;
#else
  voidfuncptr f = &cbench_func_00052;
#endif

CBENCH_WORKLOAD;

#if (51 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (51 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (52 <= FUNC_RETURN_PTR)
int *
#elif (52 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00052(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (53 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00053;
#elif (53 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00053;
#else
  voidfuncptr f = &cbench_func_00053;
#endif

CBENCH_WORKLOAD;

#if (52 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (52 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (53 <= FUNC_RETURN_PTR)
int *
#elif (53 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00053(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (54 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00054;
#elif (54 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00054;
#else
  voidfuncptr f = &cbench_func_00054;
#endif

CBENCH_WORKLOAD;

#if (53 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (53 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (54 <= FUNC_RETURN_PTR)
int *
#elif (54 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00054(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (55 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00055;
#elif (55 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00055;
#else
  voidfuncptr f = &cbench_func_00055;
#endif

CBENCH_WORKLOAD;

#if (54 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (54 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (55 <= FUNC_RETURN_PTR)
int *
#elif (55 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00055(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (56 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00056;
#elif (56 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00056;
#else
  voidfuncptr f = &cbench_func_00056;
#endif

CBENCH_WORKLOAD;

#if (55 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (55 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (56 <= FUNC_RETURN_PTR)
int *
#elif (56 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00056(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (57 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00057;
#elif (57 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00057;
#else
  voidfuncptr f = &cbench_func_00057;
#endif

CBENCH_WORKLOAD;

#if (56 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (56 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (57 <= FUNC_RETURN_PTR)
int *
#elif (57 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00057(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (58 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00058;
#elif (58 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00058;
#else
  voidfuncptr f = &cbench_func_00058;
#endif

CBENCH_WORKLOAD;

#if (57 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (57 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (58 <= FUNC_RETURN_PTR)
int *
#elif (58 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00058(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (59 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00059;
#elif (59 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00059;
#else
  voidfuncptr f = &cbench_func_00059;
#endif

CBENCH_WORKLOAD;

#if (58 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (58 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (59 <= FUNC_RETURN_PTR)
int *
#elif (59 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00059(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (60 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00060;
#elif (60 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00060;
#else
  voidfuncptr f = &cbench_func_00060;
#endif

CBENCH_WORKLOAD;

#if (59 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (59 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (60 <= FUNC_RETURN_PTR)
int *
#elif (60 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00060(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (61 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00061;
#elif (61 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00061;
#else
  voidfuncptr f = &cbench_func_00061;
#endif

CBENCH_WORKLOAD;

#if (60 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (60 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (61 <= FUNC_RETURN_PTR)
int *
#elif (61 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00061(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (62 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00062;
#elif (62 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00062;
#else
  voidfuncptr f = &cbench_func_00062;
#endif

CBENCH_WORKLOAD;

#if (61 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (61 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (62 <= FUNC_RETURN_PTR)
int *
#elif (62 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00062(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (63 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00063;
#elif (63 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00063;
#else
  voidfuncptr f = &cbench_func_00063;
#endif

CBENCH_WORKLOAD;

#if (62 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (62 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (63 <= FUNC_RETURN_PTR)
int *
#elif (63 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00063(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (64 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00064;
#elif (64 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00064;
#else
  voidfuncptr f = &cbench_func_00064;
#endif

CBENCH_WORKLOAD;

#if (63 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (63 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (64 <= FUNC_RETURN_PTR)
int *
#elif (64 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00064(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (65 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00065;
#elif (65 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00065;
#else
  voidfuncptr f = &cbench_func_00065;
#endif

CBENCH_WORKLOAD;

#if (64 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (64 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (65 <= FUNC_RETURN_PTR)
int *
#elif (65 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00065(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (66 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00066;
#elif (66 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00066;
#else
  voidfuncptr f = &cbench_func_00066;
#endif

CBENCH_WORKLOAD;

#if (65 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (65 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (66 <= FUNC_RETURN_PTR)
int *
#elif (66 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00066(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (67 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00067;
#elif (67 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00067;
#else
  voidfuncptr f = &cbench_func_00067;
#endif

CBENCH_WORKLOAD;

#if (66 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (66 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (67 <= FUNC_RETURN_PTR)
int *
#elif (67 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00067(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (68 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00068;
#elif (68 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00068;
#else
  voidfuncptr f = &cbench_func_00068;
#endif

CBENCH_WORKLOAD;

#if (67 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (67 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (68 <= FUNC_RETURN_PTR)
int *
#elif (68 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00068(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (69 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00069;
#elif (69 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00069;
#else
  voidfuncptr f = &cbench_func_00069;
#endif

CBENCH_WORKLOAD;

#if (68 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (68 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (69 <= FUNC_RETURN_PTR)
int *
#elif (69 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00069(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (70 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00070;
#elif (70 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00070;
#else
  voidfuncptr f = &cbench_func_00070;
#endif

CBENCH_WORKLOAD;

#if (69 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (69 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (70 <= FUNC_RETURN_PTR)
int *
#elif (70 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00070(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (71 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00071;
#elif (71 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00071;
#else
  voidfuncptr f = &cbench_func_00071;
#endif

CBENCH_WORKLOAD;

#if (70 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (70 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (71 <= FUNC_RETURN_PTR)
int *
#elif (71 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00071(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (72 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00072;
#elif (72 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00072;
#else
  voidfuncptr f = &cbench_func_00072;
#endif

CBENCH_WORKLOAD;

#if (71 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (71 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (72 <= FUNC_RETURN_PTR)
int *
#elif (72 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00072(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (73 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00073;
#elif (73 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00073;
#else
  voidfuncptr f = &cbench_func_00073;
#endif

CBENCH_WORKLOAD;

#if (72 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (72 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (73 <= FUNC_RETURN_PTR)
int *
#elif (73 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00073(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (74 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00074;
#elif (74 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00074;
#else
  voidfuncptr f = &cbench_func_00074;
#endif

CBENCH_WORKLOAD;

#if (73 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (73 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (74 <= FUNC_RETURN_PTR)
int *
#elif (74 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00074(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (75 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00075;
#elif (75 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00075;
#else
  voidfuncptr f = &cbench_func_00075;
#endif

CBENCH_WORKLOAD;

#if (74 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (74 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (75 <= FUNC_RETURN_PTR)
int *
#elif (75 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00075(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (76 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00076;
#elif (76 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00076;
#else
  voidfuncptr f = &cbench_func_00076;
#endif

CBENCH_WORKLOAD;

#if (75 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (75 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (76 <= FUNC_RETURN_PTR)
int *
#elif (76 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00076(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (77 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00077;
#elif (77 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00077;
#else
  voidfuncptr f = &cbench_func_00077;
#endif

CBENCH_WORKLOAD;

#if (76 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (76 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (77 <= FUNC_RETURN_PTR)
int *
#elif (77 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00077(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (78 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00078;
#elif (78 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00078;
#else
  voidfuncptr f = &cbench_func_00078;
#endif

CBENCH_WORKLOAD;

#if (77 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (77 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (78 <= FUNC_RETURN_PTR)
int *
#elif (78 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00078(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (79 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00079;
#elif (79 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00079;
#else
  voidfuncptr f = &cbench_func_00079;
#endif

CBENCH_WORKLOAD;

#if (78 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (78 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (79 <= FUNC_RETURN_PTR)
int *
#elif (79 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00079(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (80 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00080;
#elif (80 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00080;
#else
  voidfuncptr f = &cbench_func_00080;
#endif

CBENCH_WORKLOAD;

#if (79 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (79 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (80 <= FUNC_RETURN_PTR)
int *
#elif (80 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00080(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (81 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00081;
#elif (81 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00081;
#else
  voidfuncptr f = &cbench_func_00081;
#endif

CBENCH_WORKLOAD;

#if (80 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (80 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (81 <= FUNC_RETURN_PTR)
int *
#elif (81 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00081(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (82 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00082;
#elif (82 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00082;
#else
  voidfuncptr f = &cbench_func_00082;
#endif

CBENCH_WORKLOAD;

#if (81 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (81 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (82 <= FUNC_RETURN_PTR)
int *
#elif (82 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00082(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (83 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00083;
#elif (83 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00083;
#else
  voidfuncptr f = &cbench_func_00083;
#endif

CBENCH_WORKLOAD;

#if (82 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (82 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (83 <= FUNC_RETURN_PTR)
int *
#elif (83 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00083(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (84 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00084;
#elif (84 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00084;
#else
  voidfuncptr f = &cbench_func_00084;
#endif

CBENCH_WORKLOAD;

#if (83 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (83 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (84 <= FUNC_RETURN_PTR)
int *
#elif (84 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00084(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (85 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00085;
#elif (85 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00085;
#else
  voidfuncptr f = &cbench_func_00085;
#endif

CBENCH_WORKLOAD;

#if (84 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (84 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (85 <= FUNC_RETURN_PTR)
int *
#elif (85 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00085(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (86 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00086;
#elif (86 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00086;
#else
  voidfuncptr f = &cbench_func_00086;
#endif

CBENCH_WORKLOAD;

#if (85 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (85 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (86 <= FUNC_RETURN_PTR)
int *
#elif (86 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00086(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (87 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00087;
#elif (87 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00087;
#else
  voidfuncptr f = &cbench_func_00087;
#endif

CBENCH_WORKLOAD;

#if (86 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (86 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (87 <= FUNC_RETURN_PTR)
int *
#elif (87 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00087(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (88 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00088;
#elif (88 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00088;
#else
  voidfuncptr f = &cbench_func_00088;
#endif

CBENCH_WORKLOAD;

#if (87 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (87 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (88 <= FUNC_RETURN_PTR)
int *
#elif (88 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00088(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (89 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00089;
#elif (89 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00089;
#else
  voidfuncptr f = &cbench_func_00089;
#endif

CBENCH_WORKLOAD;

#if (88 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (88 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (89 <= FUNC_RETURN_PTR)
int *
#elif (89 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00089(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (90 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00090;
#elif (90 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00090;
#else
  voidfuncptr f = &cbench_func_00090;
#endif

CBENCH_WORKLOAD;

#if (89 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (89 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (90 <= FUNC_RETURN_PTR)
int *
#elif (90 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00090(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (91 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00091;
#elif (91 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00091;
#else
  voidfuncptr f = &cbench_func_00091;
#endif

CBENCH_WORKLOAD;

#if (90 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (90 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (91 <= FUNC_RETURN_PTR)
int *
#elif (91 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00091(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (92 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00092;
#elif (92 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00092;
#else
  voidfuncptr f = &cbench_func_00092;
#endif

CBENCH_WORKLOAD;

#if (91 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (91 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (92 <= FUNC_RETURN_PTR)
int *
#elif (92 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00092(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (93 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00093;
#elif (93 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00093;
#else
  voidfuncptr f = &cbench_func_00093;
#endif

CBENCH_WORKLOAD;

#if (92 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (92 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (93 <= FUNC_RETURN_PTR)
int *
#elif (93 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00093(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (94 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00094;
#elif (94 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00094;
#else
  voidfuncptr f = &cbench_func_00094;
#endif

CBENCH_WORKLOAD;

#if (93 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (93 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (94 <= FUNC_RETURN_PTR)
int *
#elif (94 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00094(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (95 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00095;
#elif (95 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00095;
#else
  voidfuncptr f = &cbench_func_00095;
#endif

CBENCH_WORKLOAD;

#if (94 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (94 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (95 <= FUNC_RETURN_PTR)
int *
#elif (95 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00095(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (96 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00096;
#elif (96 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00096;
#else
  voidfuncptr f = &cbench_func_00096;
#endif

CBENCH_WORKLOAD;

#if (95 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (95 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (96 <= FUNC_RETURN_PTR)
int *
#elif (96 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00096(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (97 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00097;
#elif (97 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00097;
#else
  voidfuncptr f = &cbench_func_00097;
#endif

CBENCH_WORKLOAD;

#if (96 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (96 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (97 <= FUNC_RETURN_PTR)
int *
#elif (97 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00097(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (98 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00098;
#elif (98 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00098;
#else
  voidfuncptr f = &cbench_func_00098;
#endif

CBENCH_WORKLOAD;

#if (97 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (97 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (98 <= FUNC_RETURN_PTR)
int *
#elif (98 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00098(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (99 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00099;
#elif (99 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00099;
#else
  voidfuncptr f = &cbench_func_00099;
#endif

CBENCH_WORKLOAD;

#if (98 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (98 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (99 <= FUNC_RETURN_PTR)
int *
#elif (99 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00099(ARGS_LIST)
{
  CBENCH_PROLOG;
#if (100 <= FUNC_RETURN_PTR)
  ptrfuncptr f = &cbench_func_00100;
#elif (100 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  intfuncptr f = &cbench_func_00100;
#else
  voidfuncptr f = &cbench_func_00100;
#endif

CBENCH_WORKLOAD;

#if (99 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (99 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

#if (100 <= FUNC_RETURN_PTR)
int *
#elif (100 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
int
#else
void
#endif
cbench_func_00100(ARGS_LIST)
{
  CBENCH_PROLOG;
  voidfuncptr f = &cbench_func_vleaf;
CBENCH_WORKLOAD;

#if (100 <= FUNC_RETURN_PTR)
  CBENCH_EPILOG_RET_PTR;
#elif (100 <= FUNC_RETURN_PTR + FUNC_RETURN_INT)
  CBENCH_EPILOG_RET_INT;
#else
  CBENCH_EPILOG_VOID;
#endif

}

