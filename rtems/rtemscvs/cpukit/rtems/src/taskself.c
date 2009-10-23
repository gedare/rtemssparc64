/*
 *  RTEMS Task Manager - Get ID of Self
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskself.c,v 1.1 2007/12/20 21:51:23 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/tasks.h>

rtems_id rtems_task_self(void)
{
   return _Thread_Executing->Object.id;
}
