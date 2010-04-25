/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspreset_fatal.c,v 1.1 2008/09/22 21:50:54 joel Exp $
 */

#include <rtems.h>

void bsp_reset( void )
{
  rtems_fatal_error_occurred(0);
}
