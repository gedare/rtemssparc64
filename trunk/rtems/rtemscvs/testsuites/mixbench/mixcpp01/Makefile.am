##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = mixcpp01
mixcpp01_SOURCES = init.c SimpleClass.cpp DerivedClass.cpp system.h ../../common/allow.c
mixcpp01_LDLIBS = -lstdc++

#dist_rtems_tests_DATA = mixcpp01.scn
#dist_rtems_tests_DATA += mixcpp01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

mixcpp01_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(mixcpp01_OBJECTS) $(mixcpp01_LDADD)
LINK_LIBS = $(mixcpp01_LDLIBS)

mixcpp01$(EXEEXT): $(mixcpp01_OBJECTS) $(mixcpp01_DEPENDENCIES)
	@rm -f mixcpp01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
