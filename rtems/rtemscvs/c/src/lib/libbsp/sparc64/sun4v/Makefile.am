##
## $Id$
##

ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am
include $(top_srcdir)/../../bsp.am

include_bspdir = $(includedir)/bsp

dist_project_lib_DATA = bsp_specs

include_HEADERS = include/bsp.h 
include_HEADERS += include/tm27.h
include_HEADERS += ../shared/include/asm.h
include_HEADERS += ../helenos/include/align.h
include_HEADERS += ../helenos/include/balloc.h
include_HEADERS += ../helenos/include/gentypes.h   
include_HEADERS += ../helenos/include/main.h 
include_HEADERS += ../helenos/include/ofwarch.h
include_HEADERS += ../helenos/include/ofw.h
include_HEADERS += ../helenos/include/ofw_tree.h
include_HEADERS += ../helenos/include/register.h
include_HEADERS += ../helenos/include/stack.h
include_HEADERS += ../helenos/include/types.h

nodist_include_HEADERS = include/bspopts.h    
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h 
DISTCLEANFILES = include/bspopts.h
noinst_PROGRAMS =

nodist_include_HEADERS += ../../shared/include/coverhd.h


EXTRA_DIST = ../shared/start/start.S ../shared/asm/asm.S
start.$(OBJEXT): ../shared/start/start.S
	$(CPPASCOMPILE) -o $@ -c $<
asm.$(OBJEXT): ../shared/asm/asm.S
	$(CPPASCOMPILE) -o $@ -c $<
bspinit.$(OBJEXT): start/bspinit.S
	$(CPPASCOMPILE) -o $@ -c $<
project_lib_DATA = start.$(OBJEXT) asm.$(OBJEXT) bspinit.$(OBJEXT)

dist_project_lib_DATA += ../shared/startup/linkcmds

startup_SOURCES = ../../shared/bspclean.c ../../shared/bsplibc.c \
    ../../shared/bsppredriverhook.c ../../shared/bsppretaskinghook.c \
    ../../shared/bsppost.c \
    ../../shared/bootcard.c ../../shared/sbrk.c \
    ../shared/startup/bspgetworkarea.c \
    ../shared/startup/bspstart.c \
    ../shared/startup/setvec.c \
    ../../shared/gnatinstallhandler.c  \
		../helenos/genarch/balloc.c \
    ../helenos/genarch/ofw.c \
		../helenos/genarch/ofw_tree.c \
		../helenos/generic/string.c \
		../helenos/loader/ofwarch.c \
		../helenos/loader/main.c \
		../helenos/loader/ofwasm.S \
		../shared/asm/asm.S \
	  start/bspinit.S
##   ../shared/ofw/printf.c \

#clock_SOURCES = ../../shared/clock_driver_simidle.c
clock_SOURCES = clock/ckinit.c

console_SOURCES = ../../shared/console.c  console/conscfg.c

timer_SOURCES = ../../shared/timerstub.c

noinst_LIBRARIES = libbsp.a
libbsp_a_SOURCES = $(startup_SOURCES) $(clock_SOURCES) $(console_SOURCES) \
    $(timer_SOURCES)

libbsp_a_LIBADD = \
			../../../libcpu/@RTEMS_CPU@/shared/shared-score.rel \
			../../../libcpu/@RTEMS_CPU@/shared/cache.rel \
		  ../../../libcpu/@RTEMS_CPU@/shared/interrupts.rel \
		  ../../../libcpu/@RTEMS_CPU@/@RTEMS_CPU_MODEL@/sparc-syscall.rel \
			../../../libcpu/@RTEMS_CPU@/@RTEMS_CPU_MODEL@/score.rel

EXTRA_DIST += times

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
