# $Id: Makefile.am,v 1.32 2010/04/06 12:01:28 ralf Exp $

TARGET = mipstx39-rtems4.10

include $(top_srcdir)/mkspec.am

# We only want a gdb with a tx39 simulator in it.  We share the other tools
GDB_VERS = 7.1
GDB_PKGVERS = $(GDB_VERS)
GDB_RPMREL = 3
include ../gdb.am
GDB_OPTS +=
