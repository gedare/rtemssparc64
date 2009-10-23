##
## $Id: Makefile.am,v 1.1 2009/05/14 14:29:40 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = spprintk
spprintk_SOURCES = init.c

dist_rtems_tests_DATA = spprintk.scn

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

spprintk_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(spprintk_OBJECTS) $(spprintk_LDADD)
LINK_LIBS = $(spprintk_LDLIBS)

spprintk$(EXEEXT): $(spprintk_OBJECTS) $(spprintk_DEPENDENCIES)
	@rm -f spprintk$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
