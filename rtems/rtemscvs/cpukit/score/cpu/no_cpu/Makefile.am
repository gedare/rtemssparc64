##
## $Id: Makefile.am,v 1.36 2006/01/12 09:57:43 ralf Exp $
##

include $(top_srcdir)/automake/compile.am

include_rtemsdir = $(includedir)/rtems
include_rtems_HEADERS = rtems/asm.h

include_rtems_scoredir = $(includedir)/rtems/score
include_rtems_score_HEADERS = rtems/score/cpu.h rtems/score/no_cpu.h \
    rtems/score/cpu_asm.h rtems/score/types.h

noinst_LIBRARIES = libscorecpu.a
libscorecpu_a_SOURCES = cpu.c cpu_asm.c
libscorecpu_a_CPPFLAGS = $(AM_CPPFLAGS)

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
