#
#  Config file for LPC24XX.
#
#  $Id: lpc24xx.inc,v 1.3 2009/11/03 11:34:05 ralf Exp $
#

include $(RTEMS_ROOT)/make/custom/default.cfg

RTEMS_CPU = arm

CPU_CFLAGS = -mcpu=arm7tdmi-s -mstructure-size-boundary=8 -mthumb

CFLAGS_OPTIMIZE_V = -Os -g
