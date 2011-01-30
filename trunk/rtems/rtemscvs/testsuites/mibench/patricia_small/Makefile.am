##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = patricia
patricia_SOURCES = init.c patricia.c patricia_test.c FileSystemImage.c

#dist_rtems_tests_DATA = patricia.scn
#dist_rtems_tests_DATA += patricia.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

patricia_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(patricia_OBJECTS) $(patricia_LDADD)
LINK_LIBS = $(patricia_LDLIBS)

patricia$(EXEEXT): $(patricia_OBJECTS) $(patricia_DEPENDENCIES)
	@rm -f patricia$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
