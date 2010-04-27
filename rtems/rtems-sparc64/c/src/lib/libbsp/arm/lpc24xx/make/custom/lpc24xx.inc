#
#  Config file for LPC24XX.
#
#  $Id: lpc24xx.inc,v 1.5 2010/04/09 15:02:39 thomas Exp $
#

include $(RTEMS_ROOT)/make/custom/default.cfg

RTEMS_CPU = arm

CPU_CFLAGS = -mcpu=arm7tdmi-s -mthumb -mstructure-size-boundary=8

CFLAGS_OPTIMIZE_V = -Os -g
