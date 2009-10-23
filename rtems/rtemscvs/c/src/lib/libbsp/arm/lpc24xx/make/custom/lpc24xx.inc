#
#  Config file for LPC24XX.
#
#  $Id: lpc24xx.inc,v 1.1 2009/10/17 15:21:06 ralf Exp $
#

include $(RTEMS_ROOT)/make/custom/default.cfg

RTEMS_CPU = arm

RTEMS_BSP_FAMILY = lpc24xx

CPU_CFLAGS = -mcpu=arm7tdmi-s -mstructure-size-boundary=8 -mthumb

CFLAGS_OPTIMIZE_V = -Os -g

LDFLAGS += -qnolinkcmds -T $(LPC24XX_LINKCMDS) 
