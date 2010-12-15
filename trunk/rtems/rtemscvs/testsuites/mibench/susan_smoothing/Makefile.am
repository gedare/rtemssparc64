##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = susan_smoothing
susan_smoothing_SOURCES = init.c FileSystemImage.c

#dist_rtems_tests_DATA = susan_smoothing.scn
#dist_rtems_tests_DATA += susan_smoothing.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

susan_smoothing_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
susan_smoothing_LDLIBS = -lm

LINK_OBJS = $(susan_smoothing_OBJECTS) $(susan_smoothing_LDADD)
LINK_LIBS = $(susan_smoothing_LDLIBS)

susan_smoothing$(EXEEXT): $(susan_smoothing_OBJECTS) $(susan_smoothing_DEPENDENCIES)
	@rm -f susan_smoothing$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
