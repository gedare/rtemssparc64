##
## $Id: Makefile.am,v 1.1 2010/01/19 08:55:18 thomas Exp $
##

MANAGERS = io semaphore event

rtems_tests_PROGRAMS = block09
block09_SOURCES = init.c

dist_rtems_tests_DATA = block09.scn block09.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

block09_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(block09_OBJECTS) $(block09_LDADD)
LINK_LIBS = $(block09_LDLIBS)

block09$(EXEEXT): $(block09_OBJECTS) $(block09_DEPENDENCIES)
	@rm -f block09$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
