##
## $Id$
##

ACLOCAL_AMFLAGS = -I ../aclocal

SUBDIRS = bitcount_large bitcount_small crc dijkstra \
					fft_large fft_small fftinv_large fftinv_small \
					qsort_small sha_large sha_small \
					stringsearch_large stringsearch_small \
					susan_corners susan_edges susan_smoothing

if MPTESTS
endif

if CXXTESTS
endif

if NETTESTS
SUBDIRS += patricia_small
endif

include $(top_srcdir)/../automake/subdirs.am
include $(top_srcdir)/../automake/local.am
