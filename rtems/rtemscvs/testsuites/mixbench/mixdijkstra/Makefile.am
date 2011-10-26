##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = mixdijkstra
#dijkstra_SOURCES = init.c system.h FileSystemImage.c ../../mibench/crc/Permission.c ../../mibench/crc/PermissionStack.c ../../mibench/crc/crc_32.c ../../mibench/qsort_small/qsort_small.c ../../mibench/susan_corners/susan.c
mixdijkstra_SOURCES = init.c system.h FileSystemImage.c ../../common/allow.c ../../mibench/dijkstra/dijkstra.c  

#dist_rtems_tests_DATA = mixdijkstra.scn
#dist_rtems_tests_DATA += mixdijkstra.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

mixdijkstra_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
mixdijkstra_LDLIBS = -lm

AM_CPPFLAGS += -I$(top_srcdir)/../support/include -I$(top_srcdir)/../common/

LINK_OBJS = $(mixdijkstra_OBJECTS) $(mixdijkstra_LDADD)
LINK_LIBS = $(mixdijkstra_LDLIBS)

mixdijkstra$(EXEEXT): $(mixdijkstra_OBJECTS) $(mixdijkstra_DEPENDENCIES)
	@rm -f mixdijkstra$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
