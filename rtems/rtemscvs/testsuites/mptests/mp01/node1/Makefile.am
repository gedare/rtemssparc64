##
## $Id: Makefile.am,v 1.32 2009/04/03 12:55:13 ralf Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = mp01-node1
mp01_node1_SOURCES = ../init.c ../task1.c ../system.h

dist_rtems_tests_DATA = mp01-node1.scn
dist_rtems_tests_DATA += mp01-node1.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

AM_CPPFLAGS += -DNODE_NUMBER=1 -I$(srcdir)/..
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

mp01_node1_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(mp01_node1_OBJECTS) $(mp01_node1_LDADD)
LINK_LIBS = $(mp01_node1_LDLIBS)

mp01-node1$(EXEEXT): $(mp01_node1_OBJECTS) $(mp01_node1_DEPENDENCIES)
	@rm -f mp01-node1$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
