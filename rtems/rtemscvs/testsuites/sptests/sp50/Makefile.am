##
## $Id: Makefile.am,v 1.1 2009/05/21 15:42:52 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = sp50
sp50_SOURCES = init.c

dist_rtems_tests_DATA = sp50.scn
dist_rtems_tests_DATA += sp50.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sp50_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(sp50_OBJECTS) $(sp50_LDADD)
LINK_LIBS = $(sp50_LDLIBS)

sp50$(EXEEXT): $(sp50_OBJECTS) $(sp50_DEPENDENCIES)
	@rm -f sp50$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
