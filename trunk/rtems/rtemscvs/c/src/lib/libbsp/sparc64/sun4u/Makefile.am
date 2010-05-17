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
include_HEADERS += ../shared/include/traptable.h 

include_bootdir = $(includedir)/boot
include_boot_HEADERS = ../helenos/boot/include/align.h \
  ../helenos/boot/include/balloc.h \
  ../helenos/boot/include/gentypes.h \
  ../helenos/boot/include/main.h \
  ../helenos/boot/include/ofwarch.h \
  ../helenos/boot/include/ofw.h \
  ../helenos/boot/include/ofw_tree.h \
  ../helenos/boot/include/register.h \
  ../helenos/boot/include/stack.h \
  ../helenos/boot/include/types.h 

## These include files mimic the HelenOS kernel include layout, which 
## in HelenOS-0.4.2 is more complicated than the boot include dirs.
include_archdir = $(includedir)/arch
include_arch_HEADERS = ../helenos/kernel/sparc64/include/boot.h \
	../helenos/kernel/sparc64/include/barrier.h \
	../helenos/kernel/sparc64/include/regdef.h \
	../helenos/kernel/sparc64/include/register.h \
	../helenos/kernel/sparc64/include/stack.h \
  ../helenos/kernel/sparc64/include/sun4u/arch.h \
	../helenos/kernel/sparc64/include/sun4u/cpu.h

include_arch_mmdir = $(includedir)/arch/mm/
include_arch_mm_HEADERS = ../helenos/kernel/sparc64/include/sun4u/mmu.h \
	../helenos/kernel/sparc64/include/sun4u/page.h \
	../helenos/kernel/sparc64/include/sun4u/tlb.h \
	../helenos/kernel/sparc64/include/sun4u/tte.h

include_genarch_ofwdir = $(includedir)/genarch/ofw
include_genarch_ofw_HEADERS = ../helenos/kernel/genarch/include/ofw/ofw_tree.h


nodist_include_HEADERS = include/bspopts.h    
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h 
DISTCLEANFILES = include/bspopts.h
noinst_PROGRAMS =

nodist_include_HEADERS += ../../shared/include/coverhd.h


EXTRA_DIST = ../shared/start/start.S 
start.$(OBJEXT): ../shared/start/start.S
	$(CPPASCOMPILE) -o $@ -c $<
asm.$(OBJEXT): ../shared/asm/asm.S
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
		../helenos/boot/genarch/balloc.c \
    ../helenos/boot/genarch/ofw.c \
		../helenos/boot/genarch/ofw_tree.c \
		../helenos/boot/generic/string.c \
		../helenos/boot/sparc64/loader/ofwarch.c \
		../helenos/boot/sparc64/loader/main.c \
		../helenos/boot/sparc64/loader/ofwasm.S \
		../shared/asm/asm.S \
		../shared/start/trap_table.S \
		start/bspinit.c \
		../helenos/kernel/sparc64/src/sun4u/takemmu.S
##   ../shared/ofw/printf.c \

clock_SOURCES = ../../shared/clock_driver_simidle.c
#clock_SOURCES = clock/ckinit.c

console_SOURCES = ../../shared/console.c  ../sun4v/console/conscfg.c

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
