##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = fftinv_large
fftinv_large_SOURCES = init.c system.h fftmisc.c  fourierf.c  main.c ddc.h  ddcmath.h  fourier.h


#dist_rtems_tests_DATA = fftinv_large.scn
#dist_rtems_tests_DATA += fftinv_large.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

fftinv_large_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
fftinv_large_LDLIBS = -lm

LINK_OBJS = $(fftinv_large_OBJECTS) $(fftinv_large_LDADD)
LINK_LIBS = $(fftinv_large_LDLIBS)

fftinv_large$(EXEEXT): $(fftinv_large_OBJECTS) $(fftinv_large_DEPENDENCIES)
	@rm -f fftinv_large$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
