##
## $Id: Makefile.am,v 1.1 2009/07/16 15:00:44 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psxautoinit02
psxautoinit02_SOURCES = init.c ../include/pmacros.h

scndir = $(rtems_testsdir)
dist_rtems_tests_DATA = psxautoinit02.scn
dist_rtems_tests_DATA += psxautoinit02.doc


include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psxautoinit02_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psxautoinit02_OBJECTS) $(psxautoinit02_LDADD)
LINK_LIBS = $(psxautoinit02_LDLIBS)

psxautoinit02$(EXEEXT): $(psxautoinit02_OBJECTS) \
    $(psxautoinit02_DEPENDENCIES)
	@rm -f psxautoinit02$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
