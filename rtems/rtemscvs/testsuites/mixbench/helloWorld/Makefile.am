##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = helloWorld
helloWorld_SOURCES = init.c system.h ../../common/allow.c

#dist_rtems_tests_DATA = helloWorld.scn
#dist_rtems_tests_DATA += helloWorld.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

helloWorld_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(helloWorld_OBJECTS) $(helloWorld_LDADD)
LINK_LIBS = $(helloWorld_LDLIBS)

helloWorld$(EXEEXT): $(helloWorld_OBJECTS) $(helloWorld_DEPENDENCIES)
	@rm -f helloWorld$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
