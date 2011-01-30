##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = qsort_small
qsort_small_SOURCES = init.c FileSystemImage.c qsort_small.c

#dist_rtems_tests_DATA = qsort_small.scn
#dist_rtems_tests_DATA += qsort_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

qsort_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
qsort_small_LDLIBS = -lm

LINK_OBJS = $(qsort_small_OBJECTS) $(qsort_small_LDADD)
LINK_LIBS = $(qsort_small_LDLIBS)

qsort_small$(EXEEXT): $(qsort_small_OBJECTS) $(qsort_small_DEPENDENCIES)
	@rm -f qsort_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
