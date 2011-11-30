##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = iheap
iheap_SOURCES = ../shared/init.c ../shared/pqbench.h \
 									../shared/workload.c ../shared/workload.h \
									../shared/params.h ../shared/params.i \
									system.h pqbench.c \
									iheap.c iheap.h

#dist_rtems_tests_DATA = iheap.scn
#dist_rtems_tests_DATA += iheap.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

iheap_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(iheap_OBJECTS) $(iheap_LDADD)
LINK_LIBS = $(iheap_LDLIBS)

iheap$(EXEEXT): $(iheap_OBJECTS) $(iheap_DEPENDENCIES)
	@rm -f iheap$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
