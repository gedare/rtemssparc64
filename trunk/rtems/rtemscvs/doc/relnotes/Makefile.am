#
#  COPYRIGHT (c) 1988-2002.
#  On-Line Applications Research Corporation (OAR).
#  All rights reserved.
#
#  $Id: Makefile.am,v 1.11 2003/12/12 13:09:55 ralf Exp $
#

PROJECT = relnotes
EDITION = 1

include $(top_srcdir)/project.am
include $(top_srcdir)/main.am

COMMON_FILES += $(top_srcdir)/common/cpright.texi

FILES = install.texi intro.texi probrep.texi relnotes.texi status.texi

info_TEXINFOS = relnotes.texi
relnotes_TEXINFOS = $(FILES) $(COMMON_FILES)

CLEANFILES += relnotes.info
