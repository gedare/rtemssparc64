# $Id: Makefile.am,v 1.5 2010/06/09 06:59:05 ralf Exp $

rtems_api = 4.11
TARGET = mipstx39-rtems4.11

include $(top_srcdir)/mkspec.am

# We only want a gdb with a tx39 simulator in it.  We share the other tools
GDB_VERS = 7.1
GDB_PKGVERS = $(GDB_VERS)
GDB_RPMREL = 3
include ../gdb.am
GDB_OPTS +=
