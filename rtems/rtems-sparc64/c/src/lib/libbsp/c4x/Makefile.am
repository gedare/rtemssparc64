##
## $Id: Makefile.am,v 1.10 2005/10/16 08:28:54 ralf Exp $
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

## Descend into the @RTEMS_BSP_FAMILY@ directory
SUBDIRS = @RTEMS_BSP_FAMILY@

EXTRA_DIST =
EXTRA_DIST += shared/bspspuriousinit.c
EXTRA_DIST += shared/c3xspurious.c
EXTRA_DIST += shared/c4xspurious.c

include $(top_srcdir)/../../../automake/subdirs.am
include $(top_srcdir)/../../../automake/local.am
