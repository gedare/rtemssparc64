##
## $Id: Makefile.am,v 1.1 2009/08/01 00:25:20 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = sp63
sp63_SOURCES = init.c

dist_rtems_tests_DATA = sp63.scn
dist_rtems_tests_DATA += sp63.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sp63_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(sp63_OBJECTS) $(sp63_LDADD)
LINK_LIBS = $(sp63_LDLIBS)

sp63$(EXEEXT): $(sp63_OBJECTS) $(sp63_DEPENDENCIES)
	@rm -f sp63$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
