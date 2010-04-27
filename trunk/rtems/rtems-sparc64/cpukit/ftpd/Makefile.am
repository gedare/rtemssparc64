##
## $Id: Makefile.am,v 1.28 2006/11/18 02:28:35 ralf Exp $
##

include $(top_srcdir)/automake/compile.am

if LIBNETWORKING
project_lib_LIBRARIES = libftpd.a
libftpd_a_SOURCES = ftpd.c ftpd.h
endif

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
