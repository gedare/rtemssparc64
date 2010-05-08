##
## $Id: Makefile.am,v 1.14 2010/04/30 13:21:08 sh Exp $
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

## Descend into the @RTEMS_BSP_FAMILY@ directory
## Currently, the shared directory is not explicitly
## added but it is present in the source tree.
SUBDIRS = @RTEMS_BSP_FAMILY@

EXTRA_DIST =

# comm
EXTRA_DIST += shared/comm/uart.h
EXTRA_DIST += shared/comm/uart.c

# abort
EXTRA_DIST += shared/abort/simple_abort.c

include $(top_srcdir)/../../../automake/subdirs.am
include $(top_srcdir)/../../../automake/local.am
