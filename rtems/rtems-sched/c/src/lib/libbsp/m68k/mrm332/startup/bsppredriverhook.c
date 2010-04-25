/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsppredriverhook.c,v 1.1 2008/09/16 19:03:32 joel Exp $
 */

#include <bsp.h>


/*
 *  Call Spurious_Initialize in bsp_predriver_hook because
 *  bsp_predriver_hook is call after the _ISR_Vector_Table allocation
 */
void bsp_predriver_hook(void)
{
  extern void Spurious_Initialize(void);
  Spurious_Initialize();
}
