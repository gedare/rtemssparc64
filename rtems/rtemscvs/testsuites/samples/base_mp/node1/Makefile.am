##
##  $Id: Makefile.am,v 1.33 2009/10/30 13:35:47 ralf Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = base_mp-node1
base_mp_node1_SOURCES = ../init.c ../apptask.c ../system.h

dist_rtems_tests_DATA = base_mp-node1.scn
dist_rtems_tests_DATA += base_mp-node1.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

base_mp_node1_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -DNODE_NUMBER=1 -I.
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(base_mp_node1_OBJECTS) $(base_mp_node1_LDADD)
LINK_LIBS = $(base_mp_node1_LDLIBS)

base_mp-node1$(EXEEXT): $(base_mp_node1_OBJECTS) $(base_mp_node1_DEPENDENCIES)
	@rm -f base_mp-node1$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
