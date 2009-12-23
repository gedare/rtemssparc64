##
## $Id: Makefile.am,v 1.13 2009/12/07 16:24:17 ralf Exp $
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

# irq
EXTRA_DIST += shared/irq/irq_init.c

# abort
EXTRA_DIST += shared/abort/simple_abort.c

include $(top_srcdir)/../../../automake/subdirs.am
include $(top_srcdir)/../../../automake/local.am
