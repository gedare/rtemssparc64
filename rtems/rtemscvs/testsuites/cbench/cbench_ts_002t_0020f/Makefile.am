##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = cbench_ts_002t_0020f
cbench_ts_002t_0020f_SOURCES = ../shared/init.c ../shared/cbench.h \
									../shared/params.h \
									system.h ../../common/allow.c \
									generated.h generated.c ../shared/cbench_ts_002t.c

#dist_rtems_tests_DATA = cbench_ts_002t_0020f.scn
#dist_rtems_tests_DATA += cbench_ts_002t_0020f.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

cbench_ts_002t_0020f_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include -I$(top_srcdir)/../common
AM_CPPFLAGS += -I$(top_srcdir)/./ -I$(top_srcdir)/../shared

LINK_OBJS = $(cbench_ts_002t_0020f_OBJECTS) $(cbench_ts_002t_0020f_LDADD)
LINK_LIBS = $(cbench_ts_002t_0020f_LDLIBS)

cbench_ts_002t_0020f$(EXEEXT): $(cbench_ts_002t_0020f_OBJECTS) $(cbench_ts_002t_0020f_DEPENDENCIES)
	@rm -f cbench_ts_002t_0020f$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
