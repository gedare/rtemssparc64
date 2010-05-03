##
## $Id$
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

include $(top_srcdir)/../../../automake/compile.am

noinst_PROGRAMS =


if shared
include_libcpudir = $(includedir)/libcpu

include_libcpu_HEADERS = ../shared/include/cache.h

noinst_PROGRAMS += shared/interrupts.rel
shared_interrupts_rel_SOURCES = shared/interrupts/installisrentries.c
shared_interrupts_rel_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/shared/interrupts
shared_interrupts_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

noinst_PROGRAMS += shared/cache.rel
shared_cache_rel_SOURCES = shared/cache/cache.c shared/cache/cache_.h \
    ../shared/src/cache_aligned_malloc.c ../shared/src/cache_manager.c
shared_cache_rel_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/shared/cache
shared_cache_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

#if has_instruction_cache
#cache_rel_CPPFLAGS += -DHAS_INSTRUCTION_CACHE
#endif

if sun4v
# NOTE: Unlike other CPUs, we install into a subdirectory to avoid
#       file name conflicts (See sh CPU for similar approach)
include_sparc64dir = $(includedir)/sparc64
include_rtems_scoredir = $(includedir)/rtems/score

#include_sparc64_HEADERS =  include/traps.h
#include_rtems_score_HEADERS = $(include_rtems_scoredir)/sparc64.h \
#															$(include_rtems_scoredir)/cpu.h \
#															$(include_rtems_scoredir)/types.h

## sun4v/score
noinst_PROGRAMS += sun4v/score.rel
sun4v_score_rel_SOURCES = sun4v/score/interrupt.S sun4v/score/cpu.c
sun4v_score_rel_CPPFLAGS = $(AM_CPPFLAGS)
sun4v_score_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)


noinst_PROGRAMS += sun4v/sparc-syscall.rel
sun4v_sparc_syscall_rel_SOURCES = sun4v/syscall/sparc-syscall.S \
																	sun4v/syscall/sparc-syscall.h
sun4v_sparc_syscall_rel_CPPFLAGS = $(AM_CPPFLAGS)
sun4v_sparc_syscall_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

if m5sim

## m5sim/score
noinst_PROGRAMS += m5sim/score.rel
m5sim_score_rel_SOURCES = m5sim/score/cpu_asm.c m5sim/score/cpu.c
m5sim_score_rel_CPPFLAGS = $(AM_CPPFLAGS)
m5sim_score_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

noinst_PROGRAMS += m5sim/sparc-syscall.rel
m5sim_sparc_syscall_rel_SOURCES = m5sim/syscall/sparc-syscall.S \
																	m5sim/syscall/sparc-syscall.h
m5sim_sparc_syscall_rel_CPPFLAGS = $(AM_CPPFLAGS)
m5sim_sparc_syscall_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif



include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../automake/local.am
