#
#  COPYRIGHT (c) 1988-2002.
#  On-Line Applications Research Corporation (OAR).
#  All rights reserved.
#
#  $Id: Makefile.am,v 1.12 2010/06/11 07:40:00 ralf Exp $
#

PROJECT = relnotes

include $(top_srcdir)/project.am
include $(top_srcdir)/main.am

COMMON_FILES += $(top_srcdir)/common/cpright.texi

FILES = install.texi intro.texi probrep.texi relnotes.texi status.texi

info_TEXINFOS = relnotes.texi
relnotes_TEXINFOS = $(FILES) $(COMMON_FILES)

CLEANFILES += relnotes.info
