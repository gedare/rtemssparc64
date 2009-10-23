## $Id: Makefile.am,v 1.6 2008/02/01 21:28:08 joel Exp $

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../../../testsuites/automake/compile.am

include $(top_srcdir)/ada.am

noinst_PROGRAMS = ada_tm14.exe
ada_tm14_exe_SOURCES = tm14.adb config.h tmtest.adb tmtest.ads
ada_tm14_exe_SOURCES += ../../support/init.c

ada_tm14.exe: tm14.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

include $(top_srcdir)/../../../testsuites/automake/local.am
