##
## $Id: Makefile.am,v 1.2 2009/04/03 12:44:41 ralf Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psxualarm
psxualarm_SOURCES = init.c system.h ../include/pmacros.h

dist_rtems_tests_DATA = psxualarm.scn

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psxualarm_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psxualarm_OBJECTS) $(psxualarm_LDADD)
LINK_LIBS = $(psxulaarm_LDLIBS)

psxualarm$(EXEEXT): $(psxualarm_OBJECTS) $(psxualarm_DEPENDENCIES)
	@rm -f psxualarm$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
