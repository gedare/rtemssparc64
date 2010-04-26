# $Id: Makefile.am,v 1.4 2010/04/06 11:49:18 ralf Exp $

rtems_api = 4.11
TARGET = mipstx39-rtems4.11

include $(top_srcdir)/mkspec.am

# We only want a gdb with a tx39 simulator in it.  We share the other tools
GDB_VERS = 7.1
GDB_PKGVERS = $(GDB_VERS)
GDB_RPMREL = 2
include ../gdb.am
GDB_OPTS +=
