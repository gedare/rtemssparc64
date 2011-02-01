##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = neighborhood
neighborhood_SOURCES = FileSystemImage.c system.h init.c neighborhood.c utili.h

#dist_rtems_tests_DATA = neighborhood.scn
#dist_rtems_tests_DATA += neighborhood.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

neighborhood_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
neighborhood_LDLIBS = -lm

LINK_OBJS = $(neighborhood_OBJECTS) $(neighborhood_LDADD)
LINK_LIBS = $(neighborhood_LDLIBS)

neighborhood$(EXEEXT): $(neighborhood_OBJECTS) $(neighborhood_DEPENDENCIES)
	@rm -f neighborhood$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
