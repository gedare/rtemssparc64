/*
 *  Timer Manager
 *
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: no-timer.c,v 1.16 2008/12/22 04:29:18 ralf Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>
#include <rtems/score/interr.h>

#include <rtems/rtems/types.h>
#include <rtems/rtems/timer.h>

void _Timer_Manager_initialization(void)
{
}

