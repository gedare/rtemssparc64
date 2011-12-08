#!/bin/python
##
## Create a cbench test:
##    generated.c
##    generated.h
##    Makefile.am

import random
import math
import sys
import getopt
import array
import os
import copy

def usage():
  print "Usage: gencbenchtest.py -[hf:]\n\
  -h --help           print this help\n\
  -f --functions      number of functions\n"

## Simple string substitutions from parameters
def get_functions( f ):
  return "#define NUM_FUNCTIONS (" + str(f) + ")"

## usual strings to include
def get_headerbegin( filename ):
  return '\n\n#ifndef __' + filename.upper() + '_H_\n#define __' + filename.upper() + '_H_\n'

def get_cplusplusbegin():
  return '#ifdef __cplusplus\nextern "C" {\n#endif\n'

def get_cplusplusend():
  return '#ifdef __cplusplus\n}\n#endif\n'

def get_fixeddefines():
  s = ''
  s = s + '#define FUNC_RETURN_PTR (NUM_FUNCTIONS*CBENCH_PARAM_PERCENT_RETURN_REF)\n'
  s = s + '#define FUNC_RETURN_INT (NUM_FUNCTIONS*CBENCH_PARAM_PERCENT_RETURN_INT)\n'
  s = s + '#define FUNC_RETURN_VOID (NUM_FUNCTIONS*CBENCH_PARAM_PERCENT_RETURN_VOID)\n'
  return s

def get_hincludes():
  s = ''
  s = s + '#include "../shared/params.h"\n'
  s = s + '#include "../shared/cbench.h"\n'
  return s

def get_cincludes():
  return '#include "generated.h"\n'

def get_fixedprototypes():
  s = ''
  s = s + 'int* cbench_func_pleaf(ARGS_LIST);\n'
  s = s + 'int cbench_func_ileaf(ARGS_LIST);\n'
  s = s + 'void cbench_func_vleaf(ARGS_LIST);\n'
  return s

def get_pleaf():
  s = ''
  s = s + 'int* cbench_func_pleaf(ARGS_LIST) {\n'
  s = s + '  CBENCH_PROLOG;\n'
  s = s + '  __asm__ __volatile__("nop\\n\\t");\n'
  s = s + '  CBENCH_EPILOG_RET_PTR;\n'
  s = s + '}\n'
  return s

def get_ileaf():
  s = ''
  s = s + 'int cbench_func_ileaf(ARGS_LIST) {\n'
  s = s + '  CBENCH_PROLOG;\n'
  s = s + '  __asm__ __volatile__("nop\\n\\t");\n'
  s = s + '  CBENCH_EPILOG_RET_INT;\n'
  s = s + '}\n'
  return s

def get_vleaf():
  s = ''
  s = s + 'void cbench_func_vleaf(ARGS_LIST) {\n'
  s = s + '  CBENCH_PROLOG;\n'
  s = s + '  __asm__ __volatile__("nop\\n\\t");\n'
  s = s + '  CBENCH_EPILOG_VOID;\n'
  s = s + '}\n'
  return s

def get_fixedbodies():
  return get_pleaf() + '\n' + get_ileaf() + '\n' + get_vleaf() + '\n'

def get_rettype( i ):
  s = ''
  s = s + '#if (' +  str(i) + ' <= FUNC_RETURN_PTR)\n'
  s = s + 'int *\n'
  s = s + '#elif (' + str(i) + ' <= FUNC_RETURN_PTR + FUNC_RETURN_INT)\n'
  s = s + 'int\n#else\nvoid\n#endif\n'
  return s

def get_fname( i ):
  return 'cbench_func_' + '{:05}'.format(i)

def get_proto(i):
  return get_rettype(i) + get_fname(i) + '(ARGS_LIST);\n'

def get_ptr(i,f):
  s = ''
  if ( i < f ):
    s = s + '#if (' + str(i+1) + ' <= FUNC_RETURN_PTR)\n'
    s = s + '  ptrfuncptr f = &' + get_fname(i+1) + ';\n'
    s = s + '#elif (' + str(i+1) + ' <= FUNC_RETURN_PTR + FUNC_RETURN_INT)\n'
    s = s + '  intfuncptr f = &' + get_fname(i+1) + ';\n'
    s = s + '#else\n'
    s = s + '  voidfuncptr f = &' + get_fname(i+1) + ';\n'
    s = s + '#endif\n'
  else:
    s = s + '  voidfuncptr f = &cbench_func_vleaf;'
  return s

def get_return(i):
  s = ''
  s = s + '#if (' +  str(i) + ' <= FUNC_RETURN_PTR)\n'
  s = s + '  CBENCH_EPILOG_RET_PTR;\n'
  s = s + '#elif (' + str(i) + ' <= FUNC_RETURN_PTR + FUNC_RETURN_INT)\n'
  s = s + '  CBENCH_EPILOG_RET_INT;\n'
  s = s + '#else\n  CBENCH_EPILOG_VOID;\n#endif\n'
  return s

def get_body(i,f):
  s = ''
  s = s + get_rettype(i) + get_fname(i) + '(ARGS_LIST)\n{\n  CBENCH_PROLOG;\n'
  s = s + get_ptr(i,f) + '\n'
  s = s + 'CBENCH_WORKLOAD;\n\n'
  s = s + get_return(i) + '\n}\n'
  return s

def main():

  header = "/* command line: "
  functions = 0

  # Process args
  try:
    opts, args = getopt.getopt(sys.argv[1:], "hf:",
        ["help", "functions="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
      sys.exit()
    elif opt in ("-f", "--functions"):
      functions = int(arg)
    else:
      assert False, "unhandled option"
    header = header + opt + " " + arg + " "
  header = header + " */"

  ## error check inputs
  assert (functions > 0), "Error: f <= 0"

  # Create output
  h = open('generated.h', 'w')
  h.write('/* This is a generated file. DO NOT EDIT. */\n\n')
  h.write(header + '\n')
  h.write(get_headerbegin("generated") + '\n')
  h.write(get_cplusplusbegin() + '\n')
  h.write(get_functions(functions) + '\n')
  h.write(get_fixeddefines() + '\n')
  h.write(get_hincludes() + '\n')
  h.write(get_fixedprototypes() + '\n')

  for i in range(1,functions+1):
    h.write(get_proto(i) + '\n')

  h.write(get_cplusplusend() + '\n')
  h.write('#endif')

  h.close()
  print "generated.h written"

  c = open('generated.c', 'w')
  c.write('/* This is a generated file. DO NOT EDIT. */\n\n')
  c.write(header + '\n')
  c.write(get_cincludes() + '\n')
  c.write(get_fixedbodies() + '\n')

  for i in range(1,functions+1):
    c.write(get_body(i,functions) + '\n')

  c.close()
  print "generated.c written"

  m = open('Makefile.am', 'w')
  m.write('##\n##  $Id$\n##\n\nMANAGERS = all\n\n')
  testname = 'cbench_ts_' + '{:05}'.format(functions) + 'f'
  m.write('rtems_tests_PROGRAMS = ' + testname + '\n')
  m.write(testname + '_SOURCES = ../shared/init.c\n')
  m.write(testname + '_SOURCES += ../shared/cbench.h\n')
  m.write(testname + '_SOURCES += ../shared/params.h\n')
  m.write(testname + '_SOURCES += ../shared/system.h\n')
  m.write(testname + '_SOURCES += ../shared/cbench_ts.c\n')
  m.write(testname + '_SOURCES += ../../common/allow.c\n')
  m.write(testname + '_SOURCES += generated.h\n')
  m.write(testname + '_SOURCES += generated.c\n')
  m.write('\n')
  m.write('include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg\n')
  m.write('include $(top_srcdir)/../automake/compile.am\n')
  m.write('include $(top_srcdir)/../automake/leaf.am\n')
  m.write('\n')
  m.write(testname + '_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)\n')
  m.write('\n')
  m.write('AM_CPPFLAGS += -I$(top_srcdir)/../support/include\n')
  m.write('AM_CPPFLAGS += -I$(top_srcdir)/../common\n')
  m.write('AM_CPPFLAGS += -I$(top_srcdir)/\n')
  m.write('AM_CPPFLAGS += -I$(top_srcdir)/shared\n')
  m.write('\n')
  m.write('LINK_OBJS = $(' + testname + '_OBJECTS) $(' + testname + '_LDADD)\n')
  m.write('LINK_LIBS = $(' + testname + '_LDLIBS)\n')
  m.write('\n')
  m.write(testname + '$(EXEEXT): $(' + testname + '_OBJECTS) $(' + testname + '_DEPENDENCIES)\n')
  m.write('\t@rm -f ' + testname + '$(EXEEXT)\n')
  m.write('\t$(make-exe)\n')
  m.write('\n')
  m.write('include $(top_srcdir)/../automake/local.am')
  m.close()
  print "Makefile.am written"

if __name__ == "__main__":
  main()
