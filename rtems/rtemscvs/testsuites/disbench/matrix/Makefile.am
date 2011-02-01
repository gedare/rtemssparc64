##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = matrix
matrix_SOURCES = FileSystemImage.c system.h init.c matrix.c

#dist_rtems_tests_DATA = matrix.scn
#dist_rtems_tests_DATA += matrix.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

matrix_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
matrix_LDLIBS = -lm

LINK_OBJS = $(matrix_OBJECTS) $(matrix_LDADD)
LINK_LIBS = $(matrix_LDLIBS)

matrix$(EXEEXT): $(matrix_OBJECTS) $(matrix_DEPENDENCIES)
	@rm -f matrix$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
