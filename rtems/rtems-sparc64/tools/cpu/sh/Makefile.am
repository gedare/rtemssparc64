##
##  $Id: Makefile.am,v 1.10 2004/01/06 17:04:09 ralf Exp $
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

noinst_PROGRAMS = shgen

shgen_SOURCES = sci.h sci.c shgen.c

if HELP2MAN
man_MANS = shgen.1

shgen.1: shgen$(EXEEXT)
	$(HELP2MAN) -N ./shgen >$@
endif

include $(top_srcdir)/../../../automake/host.am
