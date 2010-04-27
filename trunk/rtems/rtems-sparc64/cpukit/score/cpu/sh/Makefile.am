##
## $Id: Makefile.am,v 1.39 2008/09/25 19:32:41 joel Exp $
##

include $(top_srcdir)/automake/compile.am

include_rtemsdir = $(includedir)/rtems
include_rtems_HEADERS = rtems/asm.h

include_rtems_scoredir = $(includedir)/rtems/score
include_rtems_score_HEADERS = rtems/score/cpu.h rtems/score/types.h \
    rtems/score/sh.h rtems/score/sh_io.h

noinst_LIBRARIES = libscorecpu.a
libscorecpu_a_SOURCES = cpu.c context.c
libscorecpu_a_CPPFLAGS = $(AM_CPPFLAGS)

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
