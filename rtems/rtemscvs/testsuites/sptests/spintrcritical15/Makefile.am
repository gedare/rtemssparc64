##
## $Id: Makefile.am,v 1.1 2009/07/27 22:04:19 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = spintrcritical15
spintrcritical15_SOURCES = init.c \
    ../spintrcritical_support/intrcritical.c

dist_rtems_tests_DATA = spintrcritical15.scn
dist_rtems_tests_DATA += spintrcritical15.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

spintrcritical15_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -I$(top_srcdir)/spintrcritical_support

LINK_OBJS = $(spintrcritical15_OBJECTS) $(spintrcritical15_LDADD)
LINK_LIBS = $(spintrcritical15_LDLIBS)

spintrcritical15$(EXEEXT): $(spintrcritical15_OBJECTS) $(spintrcritical15_DEPENDENCIES)
	@rm -f spintrcritical15$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
