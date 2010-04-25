/*
 * RTEMS for Nintendo DS platform initialization.
 *
 * Copyright (c) 2008 by Matthieu Bucchianeri <mbucchia@gmail.com>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 *
 * http://www.rtems.com/license/LICENSE
 *
 * $Id: bspstart.c,v 1.2 2009/07/17 13:53:04 thomas Exp $
 */

#include <bsp.h>
#include <bsp/bootcard.h>
#include <nds.h>

/*
 * start the platform.
 */

void bsp_start (void)
{
  /* initialize irq management */
  BSP_rtems_irq_mngt_init ();

  /* setup console mode for lower screen */
  irqEnable (IRQ_VBLANK);
  videoSetMode (0);
  videoSetModeSub (MODE_0_2D | DISPLAY_BG0_ACTIVE);
  vramSetBankC (VRAM_C_SUB_BG);

  SUB_BG0_CR = BG_MAP_BASE (31);
  BG_PALETTE_SUB[255] = RGB15 (31, 31, 31);
  consoleInitDefault ((u16 *) SCREEN_BASE_BLOCK_SUB (31),
                      (u16 *) CHAR_BASE_BLOCK_SUB (0), 16);

  /* print status message */
  printk ("[+] kernel console started\n");

  /* set the cpu mode to system user */
  arm_cpu_mode = 0x1f;

  /* configure clock period */
  Configuration.microseconds_per_tick = 10000;  /* us */
}

/*
 * reset bss area.
 */

void
bss_reset (void)
{
  extern uint8_t __bss_start;
  extern uint8_t __bss_end;

  memset (&__bss_start, 0, (uint32_t) & __bss_end - (uint32_t) & __bss_start);
}

/*
 * A few symbols needed by libnds but not used.
 */

#include "../include/sys/iosupport.h"
const devoptab_t *devoptab_list[STD_MAX];
void *punixTime;
