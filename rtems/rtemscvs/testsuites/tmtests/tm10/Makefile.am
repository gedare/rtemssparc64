##
## $Id: Makefile.am,v 1.26 2009/04/03 12:51:09 ralf Exp $
##

MANAGERS = io message semaphore

rtems_tests_PROGRAMS = tm10
tm10_SOURCES = task1.c system.h ../include/timesys.h \
    ../../support/src/tmtests_empty_function.c

dist_rtems_tests_DATA = tm10.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

tm10_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

OPERATION_COUNT = @OPERATION_COUNT@
AM_CPPFLAGS += -I$(top_srcdir)/include -DOPERATION_COUNT=$(OPERATION_COUNT)
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(tm10_OBJECTS) $(tm10_LDADD)
LINK_LIBS = $(tm10_LDLIBS)

tm10$(EXEEXT): $(tm10_OBJECTS) $(tm10_DEPENDENCIES)
	@rm -f tm10$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
