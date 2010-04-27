/*
 *  Support routines for TM27
 *
 *  $Id: tm27supp.c,v 1.3 2010/03/27 21:09:29 thomas Exp $
 */

#include <bsp.h>

void tm27On(void)
{
}

void tm27Off(void)
{
}

int tm27IsOn(void)
{
  return 1;
}

rtems_irq_connect_data tm27IrqData = {
  CPU_USIU_EXT_IRQ_7,
  (rtems_irq_hdl)0,
  (rtems_irq_enable)tm27On,
  (rtems_irq_disable)tm27Off,
  (rtems_irq_is_enabled)tm27IsOn
};

