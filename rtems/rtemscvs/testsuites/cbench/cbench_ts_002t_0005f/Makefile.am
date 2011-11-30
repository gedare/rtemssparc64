##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = cbench01
cbench01_SOURCES = ../shared/init.c ../shared/cbench.h \
									../shared/params.h ../shared/params.i \
									system.h cbench.c \
									generated.h generated.c

#dist_rtems_tests_DATA = cbench01.scn
#dist_rtems_tests_DATA += cbench01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

cbench01_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(cbench01_OBJECTS) $(cbench01_LDADD)
LINK_LIBS = $(cbench01_LDLIBS)

cbench01$(EXEEXT): $(cbench01_OBJECTS) $(cbench01_DEPENDENCIES)
	@rm -f cbench01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
