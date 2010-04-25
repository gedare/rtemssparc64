/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsppredriverhook.c,v 1.1 2008/09/16 19:03:46 joel Exp $
 */

#include <bsp.h>
#include <bsp/bootcard.h>

extern void bsp_spurious_initialize(void);

void bsp_predriver_hook(void)
{
  #if defined(mcpu32)
    #warning "do something about vectors!!!"
  #endif

  bsp_spurious_initialize();
}
