##
## $Id: Makefile.am,v 1.1 2009/07/20 01:12:25 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psxspin02
psxspin02_SOURCES = main.c test.c ../include/pmacros.h

scndir = $(rtems_testsdir)
dist_scn_DATA = psxspin02.scn

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psxspin02_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psxspin02_OBJECTS) $(psxspin02_LDADD)
LINK_LIBS = $(psxspin02_LDLIBS)

psxspin02$(EXEEXT): $(psxspin02_OBJECTS) \
    $(psxspin02_DEPENDENCIES)
	@rm -f psxspin02$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
