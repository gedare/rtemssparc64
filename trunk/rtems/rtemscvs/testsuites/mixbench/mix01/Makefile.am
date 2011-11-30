##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = mix01
#mix01_SOURCES = init.c system.h FileSystemImage.c ../../mibench/crc/crc_32.c ../../mibench/qsort_small/qsort_small.c ../../mibench/susan_corners/susan.c
mix01_SOURCES = init.c system.h FileSystemImage.c ../../mibench/crc/crc_32.c ../../mibench/qsort_small/qsort_small.c ../../mibench/susan_corners/susan.c ../../mibench/dijkstra/dijkstra.c ../../common/allow.c ../dhrystone/dhry_1.c ../dhrystone/dhry_2.c

#dist_rtems_tests_DATA = mix01.scn
#dist_rtems_tests_DATA += mix01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

mix01_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
mix01_LDLIBS = -lm

AM_CPPFLAGS += -I$(top_srcdir)/../support/include -I$(top_srcdir)/../mibench/crc

LINK_OBJS = $(mix01_OBJECTS) $(mix01_LDADD)
LINK_LIBS = $(mix01_LDLIBS)

mix01$(EXEEXT): $(mix01_OBJECTS) $(mix01_DEPENDENCIES)
	@rm -f mix01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
