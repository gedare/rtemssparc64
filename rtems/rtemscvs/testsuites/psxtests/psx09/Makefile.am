##
## $Id: Makefile.am,v 1.25 2009/04/03 12:44:41 ralf Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psx09
psx09_SOURCES = init.c system.h ../include/pmacros.h

dist_rtems_tests_DATA = psx09.scn

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psx09_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psx09_OBJECTS) $(psx09_LDADD)
LINK_LIBS = $(psx09_LDLIBS)

psx09$(EXEEXT): $(psx09_OBJECTS) $(psx09_DEPENDENCIES)
	@rm -f psx09$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
