/*
 * Motorola MC9328MXL Interrupt handler
 *
 * Copyright (c) 2004 by Jay Monkman <jtm@lopingdog.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.com/license/LICENSE.
 *
 *
 *  $Id: bsp_irq_init.c,v 1.6 2010/04/09 20:24:55 thomas Exp $
 */
#include <irq.h>
#include <bsp.h>

/*
 * Interrupt system initialization. Disable interrupts, clear
 * any that are pending.
 */
void BSP_rtems_irq_mngt_init(void)
{
    int i;

    for (i = 0; i < BSP_MAX_INT; i++) {
        bsp_vector_table[i].vector = default_int_handler;
        bsp_vector_table[i].data   = NULL;
    }
}

