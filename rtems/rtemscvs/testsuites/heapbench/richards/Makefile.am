##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = richards
richards_SOURCES = init.c system.h bench.c

#dist_rtems_tests_DATA = richards.scn
#dist_rtems_tests_DATA += richards.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

richards_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(richards_OBJECTS) $(richards_LDADD)
LINK_LIBS = $(richards_LDLIBS)

richards$(EXEEXT): $(richards_OBJECTS) $(richards_DEPENDENCIES)
	@rm -f richards$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
