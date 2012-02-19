##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = mixmibench
#dijkstra_SOURCES = init.c system.h FileSystemImage.c ../../mibench/crc/Permission.c ../../mibench/crc/PermissionStack.c ../../mibench/crc/crc_32.c ../../mibench/qsort_small/qsort_small.c ../../mibench/susan_corners/susan.c
mixmibench_SOURCES = init.c system.h FileSystemImage.c 	../../common/allow.c 
mixmibench_SOURCES += ../../disbench/transitive/transitive.c ../../disbench/common/DISstressmarkRNG.c 
#	../../mibench/dijkstra/dijkstra.c
#	../../mibench/qsort_small/qsort_small.c
#	../../mibench/crc/crc_32.c 
#	../../mibench/susan_corners/susan.c 
#	../../mibench/fft_small/fftmisc.c  ../../mibench/fft_small/fourierf.c ../../mibench/fft_small/main.c 
#	../../disbench/field/field.c ../../disbench/common/DISstressmarkRNG.c
#	../../disbench/transitive/transitive.c  ../../disbench/common/DISstressmarkRNG.c
#	../../heapbench/richards/bench.c 


#dist_rtems_tests_DATA = mixmibench.scn
#dist_rtems_tests_DATA += mixmibench.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

mixmibench_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
mixmibench_LDLIBS = -lm

AM_CPPFLAGS += -I$(top_srcdir)/../support/include -I$(top_srcdir)/../common/

LINK_OBJS = $(mixmibench_OBJECTS) $(mixmibench_LDADD)
LINK_LIBS = $(mixmibench_LDLIBS)

mixmibench$(EXEEXT): $(mixmibench_OBJECTS) $(mixmibench_DEPENDENCIES)
	@rm -f mixmibench$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
