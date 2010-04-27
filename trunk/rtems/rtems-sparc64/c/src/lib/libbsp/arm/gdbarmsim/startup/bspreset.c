/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspreset.c,v 1.1 2009/10/01 21:48:42 joel Exp $
 */

#include <rtems.h>
#include <bsp/bootcard.h>
#include <bsp/swi.h>

void bsp_reset( void )
{
  asm ("swi %a0" :: "i" (SWI_Exit));
}
