##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = deltablue
deltablue_SOURCES = init.c system.h Constraints.c  DeltaBlue.h  List.h           UsefulConstraints.c Constraints.h  DeltaBlue.c  List.c       TestDeltaBlue.c  UsefulConstraints.h


#dist_rtems_tests_DATA = deltablue.scn
#dist_rtems_tests_DATA += deltablue.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

deltablue_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(deltablue_OBJECTS) $(deltablue_LDADD)
LINK_LIBS = $(deltablue_LDLIBS)

deltablue$(EXEEXT): $(deltablue_OBJECTS) $(deltablue_DEPENDENCIES)
	@rm -f deltablue$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
