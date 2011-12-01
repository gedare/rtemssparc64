##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = cbench_ts_002t_0005f
cbench_ts_002t_0005f_SOURCES = ../shared/init.c ../shared/cbench.h \
									../shared/params.h ../shared/params.i \
									system.h cbench.c ../../common/allow.c \
									generated.h generated.c

#dist_rtems_tests_DATA = cbench_ts_002t_0005f.scn
#dist_rtems_tests_DATA += cbench_ts_002t_0005f.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

cbench_ts_002t_0005f_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include -I$(top_srcdir)/../common

LINK_OBJS = $(cbench_ts_002t_0005f_OBJECTS) $(cbench_ts_002t_0005f_LDADD)
LINK_LIBS = $(cbench_ts_002t_0005f_LDLIBS)

cbench_ts_002t_0005f$(EXEEXT): $(cbench_ts_002t_0005f_OBJECTS) $(cbench_ts_002t_0005f_DEPENDENCIES)
	@rm -f cbench_ts_002t_0005f$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
