##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = susan_edges
susan_edges_SOURCES = init.c FileSystemImage.c

#dist_rtems_tests_DATA = susan_edges.scn
#dist_rtems_tests_DATA += susan_edges.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

susan_edges_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(susan_edges_OBJECTS) $(susan_edges_LDADD)
LINK_LIBS = $(susan_edges_LDLIBS)

susan_edges$(EXEEXT): $(susan_edges_OBJECTS) $(susan_edges_DEPENDENCIES)
	@rm -f susan_edges$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
