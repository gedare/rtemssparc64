#!/bin/python
##
## Generate params.h for cbench
##

import random
import math
import sys
import getopt
import array
import os
import copy


def usage():
  print "Usage: genparams.py -[hi:R:V:t:a:r:v:m:f:l:]\n\
  -h --help           print this help\n\
  -i --insnmult=      instruction multiplier [1]\n\
  -R --returnref=     number of functions out of 1000 that return int* [600]\n\
  -V --returnval=     number of functions out of 1000 that return int [200]\n\
  -t --tasks=         number of tasks [2]\n\
  -a --numargs=       number of function args [4]\n\
  -r --callbyref=     number of function args to call by reference [2]\n\
  -v --callbyval=     number of function args to call by value [2]\n\
  -m --memoprate=     number of memops per 1000 iterations [200]\n\
  -f --fcallrate=     number of function calls per 1000 iterations [1]\n\
  -l --layout=        specify workload layout [1], one of:\n\
                        1. CPU-Memory-Call\n\
                        2. CPU-Call-Memory\n\
                        3. Memory-CPU-Call\n\
                        4. Memory-Call-CPU\n\
                        5. Call-CPU-Memory\n\
                        6. Call-Memory-CPU\n\
                        7. Interleaved\n"

## Simple string substitutions from parameters
def get_insnmult( i ):
  return "#define CBENCH_INSN_MULTIPLIER (" + str(i) + ")"

def get_callbyref( r ):
  return "#define CBENCH_PARAM_NUM_CALL_BY_REF (" + str(r) + ")"

def get_callbyval( v ):
  return "#define CBENCH_PARAM_NUM_CALL_BY_VALUE (" + str(v) + ")"

def get_memoprate( m ):
  return "#define CBENCH_PARAM_MEMOP_PER_1000_INSN (" + str(m) + ")"

def get_fcallrate( f ):
  return "#define CBENCH_PARAM_FUNC_CALLS_PER_1000_INSN (" + str(f) + ")"

def get_tasks( t ):
  return "#define CBENCH_NUM_TASKS (" + str(t) + ")"

## note that returnref and returnint are purposely not protected with parens
def get_returnref( R ):
  return "#define CBENCH_PARAM_PERCENT_RETURN_REF " + str(R) + "/1000"

def get_returnval( V ):
  return "#define CBENCH_PARAM_PERCENT_RETURN_INT " + str(V) + "/1000"

def get_returnvoid( R, V ):
  return "#define CBENCH_PARAM_PERCENT_RETURN_VOID " + str(1000-V-R) + "/1000"

def get_layout( l ):
  p = "#define CBENCH_WORKLOAD "
  if ( l == 1):
    return p + "CBENCH_WORKLOAD_CPU_MEM_CALL"
  elif ( l == 2):
    return p + "CBENCH_WORKLOAD_CPU_CALL_MEM"
  elif ( l == 3):
    return p + "CBENCH_WORKLOAD_MEM_CPU_CALL"
  elif ( l == 4):
    return p + "CBENCH_WORKLOAD_MEM_CALL_CPU"
  elif ( l == 5):
    return p + "CBENCH_WORKLOAD_CALL_CPU_MEM"
  elif ( l == 6):
    return p + "CBENCH_WORKLOAD_CALL_MEM_CPU"
  elif ( l == 7):
    return p + "CBENCH_WORKLOAD_INTERLEAVED"
  else:
    assert False, "Invalid layout"

## Function arguments are more involved
def get_intlist(v):
  return ['len'+str(x) for x in range(1,v+1)]

def get_reflist(r):
  return ['buf'+str(x) for x in range(1,r+1)]

def get_args_list(a,r,v):
  intlist = get_intlist(v)
  reflist = get_reflist(r)

  args_list = '#define ARGS_LIST \\\n'
  for s in reflist:
    args_list = args_list + '  int* ' + s + ', \\\n'
  for s in intlist:
    args_list = args_list + '  int ' + s + ', \\\n'
  args_list = args_list + '  int depth, \\\n'
  args_list = args_list + '  rtems_task_argument argument\n'
  
  return args_list

def get_call_list(a,r,v):
  intlist = get_intlist(v)
  reflist = get_reflist(r)
  
  call_list = '#define CALL_ARGS '
  for s in reflist:
    call_list = call_list + s + ', '
  for s in intlist:
    call_list = call_list + s + ', '
  call_list = call_list + 'depth+1, argument\n'

  return call_list

def get_intlen( s ):
#  return str(int(random.random() * 1000)) #[0,1000]
  return str(1000)

def get_task_decls(a,r,v):
  intlist = get_intlist(v)
  reflist = get_reflist(r)
  
  decls = '#define TASK_DECLARATIONS \\\n'
  for s in reflist:
    decls = decls + '  int *' + s + '; \\\n'
  for s in intlist:
    decls = decls + '  int ' + s + ' = ' + get_intlen(s) + '; \\\n'
  decls = decls + '  int depth = 0; \\\n'
  decls = decls + '  int i = 0; \\\n'
  return decls

def get_task_alloc(r,v):
  intlist = get_intlist(v)
  reflist = get_reflist(r)

  alloc = '#define TASK_ALLOCATIONS \\\n'
  for x in range(len(reflist)-1):
    alloc = alloc + '  '  + reflist[x] + ' = mymalloc(' + intlist[x] + '*sizeof(int));\\\n'
  alloc = alloc + '  ' + reflist[len(reflist)-1] + ' = mymalloc(' + intlist[len(reflist)-1] + '*sizeof(int));\n'

  return alloc

def get_task_assign(r,v):
  intlist = get_intlist(v)
  reflist = get_reflist(r)

  assign = '#define TASK_ASSIGNMENTS \\\n'
  for x in range(len(reflist)-1):
    assign = assign + '  for ( i = 0; i < ' + intlist[x] + '; i++ ) ' + reflist[x] + '[i] = 1;\\\n'
  assign = assign + '  for ( i = 0; i < ' + intlist[len(reflist)-1] + '; i++ ) ' + reflist[len(reflist)-1] + '[i] = 1;\n'

  return assign

def get_task_epilog(r):
  reflist = get_reflist(r)

  epilog = '#define CBENCH_TASK_EPILOG \\\n'
  for s in reflist:
    epilog = epilog + '  free(' + s + ');\\\n'
  epilog = epilog + '  rtems_task_delete(rtems_task_self());\n'
  return epilog

def get_allows(r,v):
  intlist = get_intlist(v)
  reflist = get_reflist(r)

  allows = '#define ALLOWS \\\n'
  for x in range(len(reflist)-1):
    allows = allows + '  ALLOW(' + reflist[x] + ', ' + intlist[x] + '*sizeof(int), DEFAULT_PERMISSION);\\\n'

  allows = allows + '  ALLOW(' + reflist[len(reflist)-1] + ', ' + intlist[len(reflist)-1] + '*sizeof(int), DEFAULT_PERMISSION);\\\n'

  return allows

def get_includes():
  return '#include <rtems.h>\n'

def get_typedefs():
  return 'typedef int* (*ptrfuncptr)(ARGS_LIST);\n\
typedef int (*intfuncptr)(ARGS_LIST);\n\
typedef void (*voidfuncptr)(ARGS_LIST);\n'

def get_task_prolog():
  return '#define CBENCH_TASK_PROLOG \\\n\
  TASK_DECLARATIONS \\\n\
  TASK_ALLOCATIONS \\\n\
  TASK_ASSIGNMENTS'

def get_fcall():
  return '#define FCALL(fptr) \\\n\
  ALLOWS\\\n\
  if ( FUNC_RETURN_PTR > depth+1 ) {\\\n\
    int * retval;\\\n\
    ptrfuncptr newfptr = fptr;\\\n\
    retval = (*newfptr)(CALL_ARGS);\\\n\
    free(retval);\\\n\
  } else if ( FUNC_RETURN_INT > FUNC_RETURN_PTR+depth+1 ) {\\\n\
    int retval;\\\n\
    intfuncptr newfptr = fptr;\\\n\
    retval = (*newfptr)(CALL_ARGS);\\\n\
  } else {\\\n\
    voidfuncptr newfptr = fptr;\\\n\
    (*newfptr)(CALL_ARGS);\\\n\
  }\n'

def main():

  header = "/* command line: "
  insnmult = 1
  returnref = 600
  returnval = 200
  tasks = 2
  numargs = 4
  callbyref = 2
  callbyval = 2
  memoprate = 200
  fcallrate = 1
  layout = 1

  # Process args
  try:
    opts, args = getopt.getopt(sys.argv[1:], "hi:R:V:t:a:r:v:m:f:l:",
        ["help", "insnmult=", "returnref=", "returnval=", "tasks=", "numargs=", "callbyref=", "callbyval=", "memoprate=", "fcallrate=", "layout="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
      sys.exit()
    elif opt in ("-i", "--insnmult"):
      insnmult = int(arg)
    elif opt in ("-R", "--returnref"):
      returnref = int(arg)
    elif opt in ("-V", "--returnval"):
      returnval = int(arg)
    elif opt in ("-t", "--tasks"):
      tasks = int(arg)
    elif opt in ("-a", "--numargs"):
      numargs = int(arg)
    elif opt in ("-r", "--callbyref"):
      callbyref = int(arg)
    elif opt in ("-v", "--callbyval"):
      callbyval = int(arg)
    elif opt in ("-m", "--memoprate"):
      memoprate = int(arg)
    elif opt in ("-f", "--fcallrate"):
      fcallrate = int(arg)
    elif opt in ("-l", "--layout"):
      layout = int(arg)
    else:
      assert False, "unhandled option"
    header = header + opt + " " + arg + " "
  header = header + " */"

  ## error check inputs
  assert (insnmult > 0), "Error: i <= 0"
  assert (tasks > 0), "Error: t <= 0"
  assert (numargs == callbyref+callbyval), "Error: n != r+v"
  assert (returnref + returnval <= 1000), "Error: R+V > 1000"
  assert (callbyref <= callbyval), "Error: callbyval must be >= callbyref"
  assert (memoprate < 1000), "Error: m >= 1000"
  assert (fcallrate < 1000), "Error: f >= 1000"
  assert (layout < 8), "Error: l > 7"

  # Create output
  f = open('params.h', 'w')
  f.write('/* This is a generated file. DO NOT EDIT. */\n\n')
  f.write(header)
  f.write('\n\n#ifndef __PARAMS_H_\n#define __PARAMS_H_\n')

  f.write(get_includes() + '\n')
  f.write(get_insnmult(insnmult) + '\n')
  f.write(get_layout(layout) + '\n')
  f.write(get_tasks(tasks) + '\n')
  f.write(get_callbyref(callbyref) + '\n')
  f.write(get_callbyval(callbyval) + '\n')
  f.write(get_fcallrate(fcallrate) + '\n')
  f.write(get_memoprate(memoprate) + '\n')
  f.write("/* Not protected with parens on purpose (arithmetic) */\n")
  f.write(get_returnref(returnref) + '\n')
  f.write(get_returnval(returnval) + '\n')
  f.write(get_returnvoid(returnref,returnval) + '\n')
  f.write('\n')
  f.write(get_args_list(numargs,callbyref,callbyval) + '\n')
  f.write(get_call_list(numargs,callbyref,callbyval) + '\n')
  f.write(get_typedefs() + '\n')
  f.write(get_task_decls(numargs,callbyref,callbyval) + '\n')
  f.write(get_task_alloc(callbyref,callbyval) + '\n')
  f.write(get_task_assign(callbyref,callbyval) + '\n')
  f.write(get_task_prolog() + '\n')
  f.write(get_task_epilog(callbyref) + '\n')
  f.write(get_allows(callbyref,callbyval) + '\n')
  f.write(get_fcall() + '\n')
  f.write('\n#endif')
  f.close()

  print "params.h written"

if __name__ == "__main__":
  main()
