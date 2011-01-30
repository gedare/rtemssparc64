##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = basicmath_small
basicmath_small_SOURCES = init.c  system.h basicmath_small.c   rad2deg.c  cubic.c   isqrt.c

#dist_rtems_tests_DATA = basicmath_small.scn
#dist_rtems_tests_DATA += basicmath_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

basicmath_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
basicmath_small_LDLIBS = -lm

LINK_OBJS = $(basicmath_small_OBJECTS) $(basicmath_small_LDADD)
LINK_LIBS = $(basicmath_small_LDLIBS)

basicmath_small$(EXEEXT): $(basicmath_small_OBJECTS) $(basicmath_small_DEPENDENCIES)
	@rm -f basicmath_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
