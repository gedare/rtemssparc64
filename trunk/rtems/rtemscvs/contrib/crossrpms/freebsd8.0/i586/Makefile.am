# $Id: Makefile.am,v 1.7 2010/05/24 07:43:03 ralf Exp $

TARGET = i586-pc-freebsd8.0

include $(top_srcdir)/mkspec.am

FREEBSD_VERS = 8.0
FREEBSD_RPMREL = 0.20091130.1
EXTRA_DIST += libs.add
EXTRA_DIST += target-libs.add
include ../libs.am

BINUTILS_VERS = 2.20.1
BINUTILS_PKGVERS = $(BINUTILS_VERS)
BINUTILS_RPMREL = 0.20100305.1
include ../binutils.am
BINUTILS_OPTS +=

GCC_VERS = 4.5.0
GCC_PKGVERS = $(GCC_VERS)
GCC_RPMREL = 0.20100524.0
include ../gcc.am
GCC_OPTS += --languages=cxx,fortran,objc
