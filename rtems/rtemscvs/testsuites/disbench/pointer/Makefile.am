##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = pointer
pointer_SOURCES = FileSystemImage.c system.h init.c pointer.c

#dist_rtems_tests_DATA = pointer.scn
#dist_rtems_tests_DATA += pointer.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

pointer_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
pointer_LDLIBS = -lm

LINK_OBJS = $(pointer_OBJECTS) $(pointer_LDADD)
LINK_LIBS = $(pointer_LDLIBS)

pointer$(EXEEXT): $(pointer_OBJECTS) $(pointer_DEPENDENCIES)
	@rm -f pointer$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
