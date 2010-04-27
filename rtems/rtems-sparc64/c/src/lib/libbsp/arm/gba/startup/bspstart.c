/**
 *  @file bspstart.c
 *
 *  This file contains the GBA BSP startup package.
 *  It includes application, board, and monitor specific initialization and
 *  configuration. The generic CPU dependent initialization has been
 *  performed before this routine is invoked.
 */
/*
 *  RTEMS GBA BSP
 *
 *  Copyright (c) 2004  Markku Puro <markku.puro@kopteri.net>
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspstart.c,v 1.12 2008/09/16 18:59:04 joel Exp $
 */

#include <stdio.h>
#include <bsp.h>
#include <rtems/bspIo.h>
#include <gba.h>
#include <conio.h>

/* External Prototypes */
extern void rtems_irq_mngt_init(void);

/** Chip registers */
volatile unsigned int *Regs = (unsigned int *)GBA_IO_REGS_ADDR;

/**
 *  @brief BSP Start
 *
 *  Called before main is invoked.
 *
 *  @param  None
 *  @return None
 */
void bsp_start_default( void )
{
  /* Init conio  */
  gba_textmode(CO60);

  /* Init rtems exceptions management
   *  !!!!!GBA -- Can't use exception vectors in GBA because they are
   *  already in GBA ROM BIOS
   */
  /* rtems_exception_init_mngt(); */

  /* Init rtems interrupt management */
  rtems_irq_mngt_init();
}

/**
 *  @brief weak alias for bsp_start_default
 *
 *  By making this a weak alias for bsp_start_default, a brave soul
 *  can override the actual bsp_start routine used.
 */
void bsp_start (void) __attribute__ ((weak, alias("bsp_start_default")));
