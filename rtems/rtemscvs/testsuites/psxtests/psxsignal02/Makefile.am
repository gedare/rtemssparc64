##
## $Id: Makefile.am,v 1.1 2009/07/24 19:18:20 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psxsignal02
psxsignal02_SOURCES = init.c ../include/pmacros.h

dist_rtems_tests_DATA = psxsignal02.scn
dist_rtems_tests_DATA += psxsignal02.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psxsignal02_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psxsignal02_OBJECTS) $(psxsignal02_LDADD)
LINK_LIBS = $(psxsignal02_LDLIBS)

psxsignal02$(EXEEXT): $(psxsignal02_OBJECTS) $(psxsignal02_DEPENDENCIES)
	@rm -f psxsignal02$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
