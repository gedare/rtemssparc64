##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = susan_corners
susan_corners_SOURCES = init.c FileSystemImage.c

#dist_rtems_tests_DATA = susan_corners.scn
#dist_rtems_tests_DATA += susan_corners.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

susan_corners_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(susan_corners_OBJECTS) $(susan_corners_LDADD)
LINK_LIBS = $(susan_corners_LDLIBS)

susan_corners$(EXEEXT): $(susan_corners_OBJECTS) $(susan_corners_DEPENDENCIES)
	@rm -f susan_corners$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
