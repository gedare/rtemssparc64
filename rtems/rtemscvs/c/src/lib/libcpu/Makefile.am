##
## $Id: Makefile.am,v 1.13 2008/09/22 14:55:15 ralf Exp $
##

include_libcpudir = $(includedir)/libcpu

EXTRA_DIST = shared/include/utility.h
EXTRA_DIST += shared/include/cache.h
EXTRA_DIST += shared/src/cache_aligned_malloc.c
EXTRA_DIST += shared/src/cache_manager.c

SUBDIRS = @libcpu_cpu_subdir@
DIST_SUBDIRS = @libcpu_cpu_subdir@

include $(top_srcdir)/automake/subdirs.am
include $(top_srcdir)/automake/local.am
