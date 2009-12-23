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
 *  $Id: umonrtemsglue.c,v 1.4 2009/11/30 22:00:47 joel Exp $
*/

#include <rtems/umon.h>

/* XXX eventually add lock/unlock methods */

static int rtems_umon_connected = 0;

void rtems_umon_connect(void)
{
  void *moncomptr;

  if ( rtems_umon_connected )
    return;

  rtems_umon_connected = 1;

  moncomptr = rtems_bsp_get_umon_monptr();
  monConnect(
    (int(*)())(*(unsigned long *)moncomptr),      /* monitor base */
    (void *)0,                                    /* lock */
    (void *)0                                     /* unlock */
  );
}
