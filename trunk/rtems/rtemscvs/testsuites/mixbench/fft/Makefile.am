##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = fft
#fft_SOURCES = init.c system.h FileSystemImage.c ../../mibench/crc/Permission.c ../../mibench/crc/PermissionStack.c ../../mibench/crc/crc_32.c ../../mibench/qsort_small/qsort_small.c ../../mibench/susan_corners/susan.c
fft_SOURCES = init.c system.h FileSystemImage.c ../../common/allow.c ../../mibench/fft_small/fftmisc.c  ../../mibench/fft_small/fourierf.c  ../../mibench/fft_small/main.c ../../mibench/fft_small/ddc.h  ../../mibench/fft_small/ddcmath.h  ../../mibench/fft_small/fourier.h

#dist_rtems_tests_DATA = fft.scn
#dist_rtems_tests_DATA += fft.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

fft_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
fft_LDLIBS = -lm

AM_CPPFLAGS += -I$(top_srcdir)/../support/include -I$(top_srcdir)/../common/

LINK_OBJS = $(fft_OBJECTS) $(fft_LDADD)
LINK_LIBS = $(fft_LDLIBS)

fft$(EXEEXT): $(fft_OBJECTS) $(fft_DEPENDENCIES)
	@rm -f fft$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
