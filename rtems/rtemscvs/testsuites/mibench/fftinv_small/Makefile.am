##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = fftinv_small
fftinv_small_SOURCES = init.c system.h fftmisc.c  fourierf.c  main.c ddc.h  ddcmath.h  fourier.h


#dist_rtems_tests_DATA = fftinv_small.scn
#dist_rtems_tests_DATA += fftinv_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

fftinv_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
fftinv_small_LDLIBS = -lm

LINK_OBJS = $(fftinv_small_OBJECTS) $(fftinv_small_LDADD)
LINK_LIBS = $(fftinv_small_LDLIBS)

fftinv_small$(EXEEXT): $(fftinv_small_OBJECTS) $(fftinv_small_DEPENDENCIES)
	@rm -f fftinv_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
