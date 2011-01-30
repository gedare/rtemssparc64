##
## $Id$
##

ACLOCAL_AMFLAGS = -I ../aclocal

SUBDIRS = bitcount_large bitcount_small crc dijkstra \
					sha_small sha_large \
					susan_corners susan_edges susan_smoothing

if MPTESTS
endif

if CXXTESTS
endif

if NETTESTS
endif

include $(top_srcdir)/../automake/subdirs.am
include $(top_srcdir)/../automake/local.am
