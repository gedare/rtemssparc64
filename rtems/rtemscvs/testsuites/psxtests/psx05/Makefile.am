##
## $Id: Makefile.am,v 1.25 2009/04/03 12:44:40 ralf Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = psx05
psx05_SOURCES = init.c task.c task2.c task3.c system.h ../include/pmacros.h

dist_rtems_tests_DATA = psx05.scn

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

psx05_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psx05_OBJECTS) $(psx05_LDADD)
LINK_LIBS = $(psx05_LDLIBS)

psx05$(EXEEXT): $(psx05_OBJECTS) $(psx05_DEPENDENCIES)
	@rm -f psx05$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
