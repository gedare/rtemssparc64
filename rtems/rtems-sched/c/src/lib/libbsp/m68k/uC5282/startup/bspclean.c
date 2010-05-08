/*
 *  SBC5206 bsp_cleanup
 *
 *  This routine returns control from RTEMS to the monitor.
 *
 *  Author: W. Eric Norum <norume@aps.anl.gov>
 *
 *  COPYRIGHT (c) 2005.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspclean.c,v 1.6 2010/04/27 17:23:22 joel Exp $
 */

#include <rtems.h>
#include <bsp.h>
#include <rtems/bspIo.h>

extern void bsp_reset(int);

void bsp_cleanup( void )
{
    bsp_reset(0);
}
