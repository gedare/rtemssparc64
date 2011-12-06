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
  print "\
Usage: genparams.py -[hi:a:p:v:m:f:l:]\n\
  -h --help           print this help\n\
  -i --insnmult=      specify instruction multiplier [1]\n\
  -a --numargs=       specify number of function args [4]\n\
  -r --callbyref=     specify number of function args to call by reference [2]\n\
  -v --callbyval=     specify number of function args to call by value [2]\n\
  -m --memoprate=     specify number of memops per 1000 iterations [200]\n\
  -f --fcallrate=     specify number of function calls per 1000 iterations [1]\n\
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
def get_args_list(a,r,v):
  return ''

def get_call_list(a,r,v):
  return ''

def get_task_decls(a,r,v):
  return ''

def get_task_alloc(r):
  return ''

def get_task_assign(r,v):
  return ''

def get_task_epilog(r):
  return ''

def get_allows(r,v):
  return ''

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
  numargs = 4
  callbyref = 2
  callbyval = 2
  memoprate = 200
  fcallrate = 1
  layout = 1

  # Process args
  try:
    opts, args = getopt.getopt(sys.argv[1:], "hi:a:r:v:m:f:l:",
        ["help", "insnmult=", "numargs=", "callbyref=", "callbyval=", "memoprate=", "fcallrate=", "layout="])
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

  # Create output
  f = open('params.h', 'w')
  f.write('/* This is a generated file. DO NOT EDIT. */\n\n')
  f.write(header)
  f.write('\n\n#ifndef __PARAMS_H_\n#define __PARAMS_H_\n')

  f.write(get_includes() + '\n')
  f.write(get_insnmult(insnmult) + '\n')
  f.write(get_layout(layout) + '\n')
  f.write(get_callbyref(callbyref) + '\n')
  f.write(get_callbyval(callbyval) + '\n')
  f.write(get_fcallrate(fcallrate) + '\n')
  f.write(get_memoprate(memoprate) + '\n')
  f.write('\n')
  f.write(get_args_list(numargs,callbyref,callbyval) + '\n')
  f.write(get_call_list(numargs,callbyref,callbyval) + '\n')
  f.write(get_typedefs() + '\n')
  f.write(get_task_decls(numargs,callbyref,callbyval) + '\n')
  f.write(get_task_alloc(callbyref) + '\n')
  f.write(get_task_assign(callbyref,callbyval) + '\n')
  f.write(get_task_epilog(callbyref) + '\n')
  f.write(get_allows(callbyref,callbyval) + '\n')
  f.write(get_fcall() + '\n')
  f.write('\n#endif')
  f.close()

  print "params.h written"

if __name__ == "__main__":
  main()
