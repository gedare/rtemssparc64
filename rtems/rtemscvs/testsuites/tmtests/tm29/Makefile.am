##
## $Id: Makefile.am,v 1.26 2009/04/03 12:51:09 ralf Exp $
##

MANAGERS = io rate_monotonic semaphore

rtems_tests_PROGRAMS = tm29
tm29_SOURCES = task1.c system.h ../include/timesys.h \
    ../../support/src/tmtests_empty_function.c

dist_rtems_tests_DATA = tm29.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

tm29_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

OPERATION_COUNT = @OPERATION_COUNT@
AM_CPPFLAGS += -I$(top_srcdir)/include -DOPERATION_COUNT=$(OPERATION_COUNT)
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(tm29_OBJECTS) $(tm29_LDADD)
LINK_LIBS = $(tm29_LDLIBS)

tm29$(EXEEXT): $(tm29_OBJECTS) $(tm29_DEPENDENCIES)
	@rm -f tm29$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
