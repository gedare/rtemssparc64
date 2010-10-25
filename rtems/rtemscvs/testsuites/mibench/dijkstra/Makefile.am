##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = dijkstra
dijkstra_SOURCES = init.c FileSystemImage.c

#dist_rtems_tests_DATA = dijkstra.scn
#dist_rtems_tests_DATA += dijkstra.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

dijkstra_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(dijkstra_OBJECTS) $(dijkstra_LDADD)
LINK_LIBS = $(dijkstra_LDLIBS)

dijkstra$(EXEEXT): $(dijkstra_OBJECTS) $(dijkstra_DEPENDENCIES)
	@rm -f dijkstra$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
