/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Modified by Fernando Nicodemos <fgnicodemos@terra.com.br>
 *  from NCB - Sistemas Embarcados Ltda. (Brazil)
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: umonsupp.c,v 1.2 2009/11/30 22:00:36 joel Exp $
 */

#include <bsp.h>

/*
 * BSP specific routine to help when calling monConnect().  This
 * returns the value known to uMon as MONCOMPTR.
 */

void *rtems_bsp_get_umon_monptr(void)
{
  return (void *)0x10000020;
}

