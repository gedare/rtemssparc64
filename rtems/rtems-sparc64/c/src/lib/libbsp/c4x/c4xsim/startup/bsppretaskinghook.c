/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsppretaskinghook.c,v 1.1 2008/09/19 15:50:44 joel Exp $
 */

#include <bsp.h>

extern void bsp_spurious_initialize();

void bsp_pretasking_hook(void)
{
  bsp_spurious_initialize();
}
