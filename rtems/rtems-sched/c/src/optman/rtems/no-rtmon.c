/*
 *  Rate Monotonic Manager
 *
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: no-rtmon.c,v 1.16 2008/12/22 04:29:18 ralf Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/score/interr.h>

#include <rtems/rtems/types.h>
#include <rtems/rtems/ratemon.h>

void _Rate_monotonic_Manager_initialization(void)
{
}
