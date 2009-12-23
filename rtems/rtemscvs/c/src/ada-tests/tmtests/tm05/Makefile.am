## $Id: Makefile.am,v 1.7 2009/12/14 18:28:02 joel Exp $

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../../../testsuites/automake/compile.am

include $(top_srcdir)/ada.am

noinst_PROGRAMS = ada_tm05
ada_tm05_SOURCES = tm05.adb config.h tmtest.adb tmtest.ads
ada_tm05_SOURCES += ../../support/init.c

ada_tm05$(EXEEXT): tm05.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

include $(top_srcdir)/../../../testsuites/automake/local.am
