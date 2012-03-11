##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = patricia_small
patricia_small_SOURCES = init.c patricia.c patricia_test.c FileSystemImage.c

#dist_rtems_tests_DATA = patricia_small.scn
#dist_rtems_tests_DATA += patricia_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

patricia_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(patricia_small_OBJECTS) $(patricia_small_LDADD)
LINK_LIBS = $(patricia_small_LDLIBS)

patricia_small$(EXEEXT): $(patricia_small_OBJECTS) $(patricia_small_DEPENDENCIES)
	@rm -f patricia_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
