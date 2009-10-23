## $Id: Makefile.am,v 1.6 2008/02/01 21:28:08 joel Exp $

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../../../testsuites/automake/compile.am

include $(top_srcdir)/ada.am

noinst_PROGRAMS = ada_tm17.exe
ada_tm17_exe_SOURCES = tm17.adb config.h tmtest.adb tmtest.ads
ada_tm17_exe_SOURCES += ../../support/init.c

ada_tm17.exe: tm17.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

include $(top_srcdir)/../../../testsuites/automake/local.am
