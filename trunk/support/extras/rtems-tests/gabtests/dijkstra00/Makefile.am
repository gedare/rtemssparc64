##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = dijkstra00
dijkstra00_SOURCES = init.c FileSystemImage.c dijkstra.c tasks_periodic.c params.h macros.h system.h

#dist_rtems_tests_DATA = dijkstra00.scn
#dist_rtems_tests_DATA += dijkstra00.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

dijkstra00_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(dijkstra00_OBJECTS) $(dijkstra00_LDADD)
LINK_LIBS = $(dijkstra00_LDLIBS)

dijkstra00$(EXEEXT): $(dijkstra00_OBJECTS) $(dijkstra00_DEPENDENCIES)
	@rm -f dijkstra00$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
