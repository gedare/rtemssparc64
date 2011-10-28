##
##  $Id: Makefile.am,v 1.26 2009/08/26 13:22:58 joel Exp $
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = dhrystone
dhrystone_SOURCES = init.c system.h dhry_1.c dhry_2.c dhry.h

#dist_rtems_tests_DATA = dhrystone.scn
#dist_rtems_tests_DATA += dhrystone.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

dhrystone_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(dhrystone_OBJECTS) $(dhrystone_LDADD)
LINK_LIBS = $(dhrystone_LDLIBS)

dhrystone$(EXEEXT): $(dhrystone_OBJECTS) $(dhrystone_DEPENDENCIES)
	@rm -f dhrystone$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

