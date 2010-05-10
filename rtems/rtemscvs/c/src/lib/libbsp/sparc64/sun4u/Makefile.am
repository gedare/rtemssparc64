##
## $Id$
##

ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am
include $(top_srcdir)/../../bsp.am

include_bspdir = $(includedir)/bsp

dist_project_lib_DATA = bsp_specs

include_HEADERS = include/bsp.h 
#include_HEADERS += include/tm27.h
#include_HEADERS += include/asm.h 
#include_HEADERS += include/balloc.h
#include_HEADERS += include/types.h
#include_HEADERS += include/ofw.h
#include_HEADERS += include/ofwarch.h
#include_HEADERS += include/gentypes.h   
#include_HEADERS += include/main.h 
#include_HEADERS += include/ofw_tree.h
#include_HEADERS += include/register.h
#include_HEADERS += include/stack.h
#include_HEADERS += include/align.h

nodist_include_HEADERS = include/bspopts.h    
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h 
DISTCLEANFILES = include/bspopts.h
noinst_PROGRAMS =

nodist_include_HEADERS += ../../shared/include/coverhd.h


EXTRA_DIST = ../shared/start/start.S 
start.$(OBJEXT): ../shared/start/start.S
	$(CPPASCOMPILE) -o $@ -c $<
asm.$(OBJEXT): ../shared/ofw/asm.S
	$(CPPASCOMPILE) -Iofw -o $@ -c $<	
project_lib_DATA = start.$(OBJEXT)

dist_project_lib_DATA += ../shared/startup/linkcmds

startup_SOURCES = ../../shared/bspclean.c ../../shared/bsplibc.c \
    ../../shared/bsppredriverhook.c ../../shared/bsppretaskinghook.c \
    ../../shared/bsppost.c \
    ../../shared/bootcard.c ../../shared/sbrk.c \
    ../shared/startup/bspgetworkarea.c \
    ../shared/startup/bspstart.c \
    ../shared/startup/setvec.c \
    ../../shared/gnatinstallhandler.c  \
    ../shared/ofw/ofw.c \
		../shared/ofw/string.c \
		../shared/ofw/balloc.c \
		../shared/ofw/bootstrap.c \
		../shared/ofw/ofwarch.c \
		../shared/ofw/ofw_tree.c \
		../shared/ofw/asm.S 
##   ../shared/ofw/printf.c \

#clock_SOURCES = ../../shared/clock_driver_simidle.c
clock_SOURCES = clock/ckinit.c

console_SOURCES = ../../shared/console.c  console/conscfg.c

timer_SOURCES = ../../shared/timerstub.c

noinst_LIBRARIES = libbsp.a
libbsp_a_SOURCES = ../shared/ofw/asm.S $(startup_SOURCES) $(clock_SOURCES) $(console_SOURCES) \
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
