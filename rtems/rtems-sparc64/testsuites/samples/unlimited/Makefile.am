##
##  $Id: Makefile.am,v 1.25 2009/10/26 11:32:35 ralf Exp $
##

MANAGERS = io event semaphore

rtems_tests_PROGRAMS = unlimited
unlimited_SOURCES = init.c test1.c test2.c test3.c system.h

dist_rtems_tests_DATA = unlimited.scn
dist_rtems_tests_DATA += unlimited.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

unlimited_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(unlimited_OBJECTS) $(unlimited_LDADD)
LINK_LIBS = $(unlimited_LDLIBS)

unlimited$(EXEEXT): $(unlimited_OBJECTS) $(unlimited_DEPENDENCIES)
	@rm -f unlimited$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
