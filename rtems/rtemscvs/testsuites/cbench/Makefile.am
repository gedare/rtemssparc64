##
## $Id: Makefile.am,v 1.17 2007/08/17 01:07:10 ccj Exp $
##

ACLOCAL_AMFLAGS = -I ../aclocal

SUBDIRS = cbench_ts_00005f
SUBDIRS += cbench_ts_00020f
SUBDIRS += cbench_ts_00100f

if MPTESTS
SUBDIRS +=
endif

if CXXTESTS
SUBDIRS +=
endif

if NETTESTS
SUBDIRS += 
endif

include $(top_srcdir)/../automake/subdirs.am
include $(top_srcdir)/../automake/local.am
