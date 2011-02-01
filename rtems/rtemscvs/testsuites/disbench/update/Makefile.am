##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = update
update_SOURCES = FileSystemImage.c system.h init.c update.c

#dist_rtems_tests_DATA = update.scn
#dist_rtems_tests_DATA += update.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

update_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
update_LDLIBS = -lm

LINK_OBJS = $(update_OBJECTS) $(update_LDADD)
LINK_LIBS = $(update_LDLIBS)

update$(EXEEXT): $(update_OBJECTS) $(update_DEPENDENCIES)
	@rm -f update$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
