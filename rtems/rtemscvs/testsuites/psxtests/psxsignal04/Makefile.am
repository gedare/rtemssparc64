##
## $Id: Makefile.am,v 1.2 2009/09/14 00:15:55 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psxsignal04
psxsignal04_SOURCES = ../psxsignal03/init.c ../include/pmacros.h

dist_rtems_tests_DATA = psxsignal04.scn
dist_rtems_tests_DATA += psxsignal04.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psxsignal04_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -DUSE_REAL_TIME_SIGNALS_PROCESS

LINK_OBJS = $(psxsignal04_OBJECTS) $(psxsignal04_LDADD)
LINK_LIBS = $(psxsignal04_LDLIBS)

psxsignal04$(EXEEXT): $(psxsignal04_OBJECTS) $(psxsignal04_DEPENDENCIES)
	@rm -f psxsignal04$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
