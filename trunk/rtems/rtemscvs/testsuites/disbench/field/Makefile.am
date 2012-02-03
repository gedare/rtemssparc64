##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = field
field_SOURCES = FileSystemImage.c system.h init.c field.c ../../common/allow.c ../common/DISstressmarkRNG.c

#dist_rtems_tests_DATA = field.scn
#dist_rtems_tests_DATA += field.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

field_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
field_LDLIBS = -lm

LINK_OBJS = $(field_OBJECTS) $(field_LDADD)
LINK_LIBS = $(field_LDLIBS)

field$(EXEEXT): $(field_OBJECTS) $(field_DEPENDENCIES)
	@rm -f field$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
