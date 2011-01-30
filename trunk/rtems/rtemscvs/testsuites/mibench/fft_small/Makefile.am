##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = fft_small
fft_small_SOURCES = init.c system.h fftmisc.c  fourierf.c  main.c ddc.h  ddcmath.h  fourier.h


#dist_rtems_tests_DATA = fft_small.scn
#dist_rtems_tests_DATA += fft_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

fft_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
fft_small_LDLIBS = -lm

LINK_OBJS = $(fft_small_OBJECTS) $(fft_small_LDADD)
LINK_LIBS = $(fft_small_LDLIBS)

fft_small$(EXEEXT): $(fft_small_OBJECTS) $(fft_small_DEPENDENCIES)
	@rm -f fft_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
