##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = basicmath_large
basicmath_large_SOURCES = init.c  system.h basicmath_large.c   rad2deg.c  cubic.c   isqrt.c

#dist_rtems_tests_DATA = basicmath_large.scn
#dist_rtems_tests_DATA += basicmath_large.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

basicmath_large_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(basicmath_large_OBJECTS) $(basicmath_large_LDADD)
LINK_LIBS = $(basicmath_large_LDLIBS)
basicmath_large_LDLIBS = -lm

basicmath_large$(EXEEXT): $(basicmath_large_OBJECTS) $(basicmath_large_DEPENDENCIES)
	@rm -f basicmath_large$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
