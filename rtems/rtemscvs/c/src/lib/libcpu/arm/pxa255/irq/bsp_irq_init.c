/*
 * PXA255 interrupt controller by Yang Xi <hiyangxi@gmail.com>
 * Copyright (c) 2004 by Jay Monkman <jtm@lopgindog.com>
 * 
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsp_irq_init.c,v 1.3 2009/08/22 13:59:55 joel Exp $
 */

#include <irq.h>
#include <bsp.h>
#include <pxa255.h>

void dummy_handler(uint32_t vector)
{
  printk("I am dummy handler\n");
}

void (*IRQ_table[PRIMARY_IRQS])(uint32_t vector);

/* 
 * Interrupt system initialization. Disable interrupts, clear 
 * any that are pending.
 */
void BSP_rtems_irq_mngt_init(void)
{
  int i;

  /* Initialize the vector table contents with default handler */
  for (i=0; i<PRIMARY_IRQS; i++) {
    IRQ_table[i] = dummy_handler;
  }

  /* disable all interrupts */
  XSCALE_INT_ICMR = 0x0;

  /* Direct the interrupt to IRQ*/
  XSCALE_INT_ICLR = 0x0;
}

