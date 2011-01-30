##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = stringsearch_small
stringsearch_small_SOURCES = init.c system.h pbmsrch_small.c bmhasrch.c bmhsrch.c bmhisrch.c search.h

#dist_rtems_tests_DATA = stringsearch_small.scn
#dist_rtems_tests_DATA += stringsearch_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

stringsearch_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(stringsearch_small_OBJECTS) $(stringsearch_small_LDADD)
LINK_LIBS = $(stringsearch_small_LDLIBS)

stringsearch_small$(EXEEXT): $(stringsearch_small_OBJECTS) $(stringsearch_small_DEPENDENCIES)
	@rm -f stringsearch_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
