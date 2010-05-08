#
#  Config file for LPC24XX.
#
#  $Id: lpc24xx.inc,v 1.6 2010/04/30 13:45:33 sh Exp $
#

include $(RTEMS_ROOT)/make/custom/default.cfg

RTEMS_CPU = arm

CPU_CFLAGS = -mstructure-size-boundary=8 -mcpu=arm7tdmi-s -mfpu=vfp -mfloat-abi=soft -mthumb

CFLAGS_OPTIMIZE_V = -Os -g
