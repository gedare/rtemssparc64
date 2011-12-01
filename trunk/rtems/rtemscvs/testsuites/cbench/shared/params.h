
#define CBENCH_PARAM_NUM_CALL_BY_REF (2)
#define CBENCH_PARAM_NUM_CALL_BY_VALUE (2)
#define CBENCH_PARAM_FUNC_CALLS_PER_100_INSN (5)
#define CBENCH_FUNC_RETURN_PTR (3)
#define CBENCH_FUNC_RETURN_INT (1)
#define CBENCH_FUNC_RETURN_VOID (1)

#define DEFAULT_PERMISSION (3LL)
#define ARGS_LIST (int* a, int* b, int c, int d)

#define DECLARATIONS \
  int *d1; \
  int *d2; \
  int s1; \
  int s2;

#define ALLOCATIONS \
  d1 = mymalloc( *sizeof(int));\
  d2 = mymalloc( *sizeof(int));

#define ASSIGNMENTS \
  *d1 = 68937693;\
  *d2 = 1928616;\
  s1 = 29373;\
  s2 = 3289073;

#define CBENCH_PROLOG \
  DECLARATIONS \
  ALLOCATIONS \
  ASSIGNMENTS

#define CBENCH_EPILOG_RET_PTR \
  ALLOW(d1, sizeof(int), DEFAULT_PERMISSION);\
  free(d2);\
  return d1;

#define CBENCH_EPILOG_RET_INT \
  free(d1);\
  free(d2);\
  return s1;

#define CBENCH_EPILOG_VOID \
  free(d1);\
  free(d2);\
  return;

#define ALLOWS \
  ALLOW(d1, sizeof(int), DEFAULT_PERMISSION);\
  ALLOW(d2, sizeof(int), DEFAULT_PERMISSION);

#define FCALL(retval, fname) \
  ALLOWS\
  retval = fname(d1, d2, s1, s2)

