# $Id: Makefile.am,v 1.15 2010/06/09 06:59:05 ralf Exp $

rtems_api = 4.11
TARGET = arm-rtems4.11

include $(top_srcdir)/mkspec.am

BINUTILS_VERS = 2.20.1
BINUTILS_PKGVERS = $(BINUTILS_VERS)
BINUTILS_RPMREL = 2
include ../binutils.am
BINUTILS_OPTS +=

GCC_VERS = 4.5.0
GCC_PKGVERS = $(GCC_VERS)
GCC_RPMREL = 13

NEWLIB_VERS = 1.18.0
NEWLIB_PKGVERS = $(NEWLIB_VERS)
NEWLIB_RPMREL = `expr $(GCC_RPMREL)`
include ../gcc.am
GCC_OPTS += --languages=cxx

GDB_VERS = 7.1
GDB_PKGVERS = $(GDB_VERS)
GDB_RPMREL = 3
include ../gdb.am
GDB_OPTS +=

CPUKIT_VERS = 4.9.99.0
CPUKIT_PKGVERS = $(CPUKIT_VERS)-20090206-2
CPUKIT_RPMREL = 1
include ../cpukit.am
CPUKIT_OPTS +=
