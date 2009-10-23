## $Id: Makefile.am,v 1.7 2007/10/25 19:51:06 humph Exp $

SUBDIRS = hello ticker base_sp nsecs

if HAS_MP
SUBDIRS += base_mp
endif
DIST_SUBDIRS = hello ticker base_sp base_mp nsecs

include $(top_srcdir)/../../../testsuites/automake/subdirs.am
include $(top_srcdir)/../../../testsuites/automake/local.am
