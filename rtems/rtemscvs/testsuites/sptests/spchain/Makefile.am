##
## $Id: Makefile.am,v 1.2 2009/07/08 20:25:30 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = spchain
spchain_SOURCES = init.c

dist_rtems_tests_DATA = spchain.scn
dist_rtems_tests_DATA += spchain.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

spchain_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(spchain_OBJECTS) $(spchain_LDADD)
LINK_LIBS = $(spchain_LDLIBS)

spchain$(EXEEXT): $(spchain_OBJECTS) $(spchain_DEPENDENCIES)
	@rm -f spchain$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
