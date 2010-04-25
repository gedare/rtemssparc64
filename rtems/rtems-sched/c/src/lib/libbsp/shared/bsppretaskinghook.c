/*
 *  This is a shared BSP pretasking hook which does nothing.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsppretaskinghook.c,v 1.3 2008/10/02 19:13:05 joel Exp $
 */

#include <bsp/bootcard.h>

void bsp_pretasking_hook(void)
{
  /* Do nothing */
}
