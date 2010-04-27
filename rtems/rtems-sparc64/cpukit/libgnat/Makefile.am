##
##  $Id: Makefile.am,v 1.4 2008/06/30 14:49:37 ralf Exp $
##

include $(top_srcdir)/automake/compile.am

EXTRA_DIST=

if LIBGNAT
AM_CPPFLAGS += -D__RTEMS_INSIDE__

noinst_LIBRARIES = libgnat.a

libgnat_a_SOURCES = ada_intrsupp.c adasupp.c
endif

include $(top_srcdir)/automake/local.am
