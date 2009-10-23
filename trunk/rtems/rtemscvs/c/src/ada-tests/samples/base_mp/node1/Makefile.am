## $Id: Makefile.am,v 1.9 2008/02/01 21:28:07 joel Exp $

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../../../testsuites/automake/compile.am

include $(top_srcdir)/ada.am

AM_ADAFLAGS += -I$(srcdir)/..

ada_base_mp_node1.exe: ../mptest.adb init.o
	$(GNATCOMPILE) -margs -a $< -o $@

init.o: ../../../support/init.c
	$(COMPILE.c) -DNODE_NUMBER=1 -I$(srcdir)/.. -c $<

if EXPADA
noinst_PROGRAMS = ada_base_mp_node1.exe
endif

scndir = $(rtems_ada_testsdir)
dist_scn_DATA = ada_base_mp-node1.scn

include $(top_srcdir)/../../../testsuites/automake/local.am
