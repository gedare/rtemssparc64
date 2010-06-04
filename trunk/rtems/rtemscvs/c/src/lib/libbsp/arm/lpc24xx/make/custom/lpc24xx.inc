#
#  Config file for LPC24XX.
#
#  $Id: lpc24xx.inc,v 1.7 2010/05/20 12:59:54 sh Exp $
#

include $(RTEMS_ROOT)/make/custom/default.cfg

RTEMS_CPU = arm

CPU_CFLAGS = -mstructure-size-boundary=8 -mcpu=arm7tdmi-s -mfpu=vfp -mfloat-abi=soft -mthumb \
	-fno-schedule-insns2

CFLAGS_OPTIMIZE_V = -Os -g
