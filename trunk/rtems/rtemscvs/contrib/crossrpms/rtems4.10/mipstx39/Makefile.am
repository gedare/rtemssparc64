# $Id: Makefile.am,v 1.26 2009/10/07 06:50:12 ralf Exp $

TARGET = mipstx39-rtems4.10

include $(top_srcdir)/mkspec.am

# We only want a gdb with a tx39 simulator in it.  We share the other tools
GDB_VERS = 7.0
GDB_PKGVERS = $(GDB_VERS)
GDB_RPMREL = 1
include ../gdb.am
GDB_OPTS +=
