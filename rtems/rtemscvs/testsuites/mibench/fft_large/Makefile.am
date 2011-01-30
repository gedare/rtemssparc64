##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = fft_large
fft_large_SOURCES = init.c system.h fftmisc.c  fourierf.c  main.c ddc.h  ddcmath.h  fourier.h


#dist_rtems_tests_DATA = fft_large.scn
#dist_rtems_tests_DATA += fft_large.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

fft_large_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
fft_large_LDLIBS = -lm

LINK_OBJS = $(fft_large_OBJECTS) $(fft_large_LDADD)
LINK_LIBS = $(fft_large_LDLIBS)

fft_large$(EXEEXT): $(fft_large_OBJECTS) $(fft_large_DEPENDENCIES)
	@rm -f fft_large$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
