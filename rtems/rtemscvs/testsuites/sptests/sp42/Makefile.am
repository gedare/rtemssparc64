##
## $Id: Makefile.am,v 1.3 2009/07/30 17:10:04 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = sp42
sp42_SOURCES = init.c

dist_rtems_tests_DATA = sp42.scn
dist_rtems_tests_DATA += sp42.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sp42_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(sp42_OBJECTS) $(sp42_LDADD)
LINK_LIBS = $(sp42_LDLIBS)

sp42$(EXEEXT): $(sp42_OBJECTS) $(sp42_DEPENDENCIES)
	@rm -f sp42$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
