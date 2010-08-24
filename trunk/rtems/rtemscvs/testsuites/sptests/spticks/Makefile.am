##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = spticks
spticks_SOURCES = init.c

dist_rtems_tests_DATA = spticks.scn
dist_rtems_tests_DATA += spticks.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

spticks_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(spticks_OBJECTS) $(spticks_LDADD)
LINK_LIBS = $(spticks_LDLIBS)

spticks$(EXEEXT): $(spticks_OBJECTS) $(spticks_DEPENDENCIES)
	@rm -f spticks$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
