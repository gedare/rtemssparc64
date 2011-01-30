##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = stringsearch_large
stringsearch_large_SOURCES = init.c system.h pbmsrch_large.c bmhasrch.c bmhsrch.c bmhisrch.c search.h

#dist_rtems_tests_DATA = stringsearch_large.scn
#dist_rtems_tests_DATA += stringsearch_large.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

stringsearch_large_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(stringsearch_large_OBJECTS) $(stringsearch_large_LDADD)
LINK_LIBS = $(stringsearch_large_LDLIBS)

stringsearch_large$(EXEEXT): $(stringsearch_large_OBJECTS) $(stringsearch_large_DEPENDENCIES)
	@rm -f stringsearch_large$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
