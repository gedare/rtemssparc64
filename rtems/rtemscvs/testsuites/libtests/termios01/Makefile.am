##
## $Id: Makefile.am,v 1.1 2009/08/15 17:54:48 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = termios01
termios01_SOURCES = init.c termios_testdriver.c

dist_rtems_tests_DATA = termios01.scn
dist_rtems_tests_DATA += termios01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

termios01_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(termios01_OBJECTS) $(termios01_LDADD)
LINK_LIBS = $(termios01_LDLIBS)

termios01$(EXEEXT): $(termios01_OBJECTS) $(termios01_DEPENDENCIES)
	@rm -f termios01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
