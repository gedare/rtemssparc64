##
## $Id: Makefile.am,v 1.14 2009/11/03 12:43:51 ralf Exp $
##

ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am

include_bspdir = $(includedir)/bsp

dist_project_lib_DATA = bsp_specs

include_HEADERS = include/bsp.h
include_HEADERS += include/tm27.h

nodist_include_HEADERS = include/bspopts.h
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h
DISTCLEANFILES = include/bspopts.h
noinst_PROGRAMS =

include_HEADERS += ../../shared/include/coverhd.h

noinst_LIBRARIES = libbspstart.a
libbspstart_a_SOURCES = start/start.S
project_lib_DATA = start.$(OBJEXT)

project_lib_DATA += startup/linkcmds
EXTRA_DIST = startup/linkcmds.m5484FireEngine.flash
EXTRA_DIST += startup/linkcmds.COBRA5475
EXTRA_DIST += startup/linkcmds.m5484FireEngine

noinst_LIBRARIES += libbsp.a
libbsp_a_SOURCES =

# startup
libbsp_a_SOURCES += ../../shared/bspclean.c ../../shared/bsplibc.c  \
  ../../shared/bsppost.c ../../shared/bspgetworkarea.c \
  ../../shared/bspreset_loop.c ../../shared/bsppretaskinghook.c \
  ../../shared/bsppredriverhook.c startup/init548x.c startup/bspstart.c \
  ../../shared/bootcard.c ../../shared/sbrk.c ../../m68k/shared/setvec.c \
  ../../shared/gnatinstallhandler.c
# clock
libbsp_a_SOURCES += clock/clock.c ../../../shared/clockdrv_shell.h
# console
libbsp_a_SOURCES += console/console.c
# timer
libbsp_a_SOURCES += timer/timer.c

if HAS_NETWORKING
network_CPPFLAGS = -D__INSIDE_RTEMS_BSD_TCPIP_STACK__
noinst_PROGRAMS += network.rel
network_rel_SOURCES = network/network.c
network_rel_CPPFLAGS = $(AM_CPPFLAGS) \
    $(network_CPPFLAGS)
network_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

libbsp_a_LIBADD = ../../../libcpu/@RTEMS_CPU@/shared/cache.rel \
                  ../../../libcpu/@RTEMS_CPU@/shared/misc.rel  \
                  ../../../libcpu/@RTEMS_CPU@/mcf548x/mcdma.rel

if HAS_NETWORKING
libbsp_a_LIBADD += network.rel
endif

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
