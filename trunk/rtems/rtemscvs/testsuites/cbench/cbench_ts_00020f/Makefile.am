##
##  $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = cbench_ts_00020f
cbench_ts_00020f_SOURCES = ../shared/init.c
cbench_ts_00020f_SOURCES += ../shared/cbench.h
cbench_ts_00020f_SOURCES += ../shared/params.h
cbench_ts_00020f_SOURCES += ../shared/system.h
cbench_ts_00020f_SOURCES += ../shared/cbench_ts.c
cbench_ts_00020f_SOURCES += ../../common/allow.c
cbench_ts_00020f_SOURCES += generated.h
cbench_ts_00020f_SOURCES += generated.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

cbench_ts_00020f_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -I$(top_srcdir)/../common
AM_CPPFLAGS += -I$(top_srcdir)/
AM_CPPFLAGS += -I$(top_srcdir)/shared

LINK_OBJS = $(cbench_ts_00020f_OBJECTS) $(cbench_ts_00020f_LDADD)
LINK_LIBS = $(cbench_ts_00020f_LDLIBS)

cbench_ts_00020f$(EXEEXT): $(cbench_ts_00020f_OBJECTS) $(cbench_ts_00020f_DEPENDENCIES)
	@rm -f cbench_ts_00020f$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am