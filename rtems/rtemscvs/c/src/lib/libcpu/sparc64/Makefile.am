##
## $Id$
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

include $(top_srcdir)/../../../automake/compile.am

noinst_PROGRAMS =

include_libcpudir = $(includedir)/libcpu
include_libcpu_HEADERS = ../shared/include/cache.h

noinst_PROGRAMS += interrupts.rel
interrupts_rel_SOURCES = interrupts/installisrentries.c
interrupts_rel_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/interrupts
interrupts_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

noinst_PROGRAMS += cache.rel
cache_rel_SOURCES = cache/cache.c cache/cache_.h \
    ../shared/src/cache_aligned_malloc.c ../shared/src/cache_manager.c
cache_rel_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/cache
cache_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

#if has_instruction_cache
#cache_rel_CPPFLAGS += -DHAS_INSTRUCTION_CACHE
#endif

if sun4v
noinst_PROGRAMS += sun4v/syscall.rel
sun4v_syscall_rel_SOURCES = sun4v/syscall/syscall.S sun4v/syscall/syscall.h
sun4v_syscall_rel_CPPFLAGS = $(AM_CPPFLAGS)
sun4v_syscall_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../automake/local.am
