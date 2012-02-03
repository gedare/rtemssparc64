##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = transitive
transitive_SOURCES = FileSystemImage.c system.h init.c transitive.c ../../common/allow.c ../common/DISstressmarkRNG.c

#dist_rtems_tests_DATA = transitive.scn
#dist_rtems_tests_DATA += transitive.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

transitive_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
transitive_LDLIBS = -lm

LINK_OBJS = $(transitive_OBJECTS) $(transitive_LDADD)
LINK_LIBS = $(transitive_LDLIBS)

transitive$(EXEEXT): $(transitive_OBJECTS) $(transitive_DEPENDENCIES)
	@rm -f transitive$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
