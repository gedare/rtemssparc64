/*
 *  Support routines for TM27
 *
 *  $Id: tm27supp.c,v 1.4 2010/04/28 17:17:48 joel Exp $
 */

#include <bsp.h>

rtems_irq_connect_data tm27IrqData = {
  CPU_USIU_EXT_IRQ_7,
  (rtems_irq_hdl)0,
  NULL,
  NULL,
  NULL
};

