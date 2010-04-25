/* irq_init.c
 *
 *  This file contains the implementation of rtems initialization
 *  related to interrupt handling.
 *
 *  CopyRight (C) 2000 Canon Research Centre France SA.
 *  Emmanuel Raguet, mailto:raguet@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsp_irq_init.c,v 1.2 2008/08/20 05:52:39 ralf Exp $
 */
#include <irq.h>
#include <bsp.h>
#include <s3c24xx.h>

extern void default_int_handler(void);

void BSP_rtems_irq_mngt_init(void)
{
    long *vectorTable;
    int i;

    vectorTable = (long *) VECTOR_TABLE;

    /* Initialize the vector table contents with default handler */
    for (i=0; i<BSP_MAX_INT; i++) {
        *(vectorTable + i) = (long)(default_int_handler);
    }

    /*
     * Here is the code to initialize the INT for
     * the specified BSP
     */
}
