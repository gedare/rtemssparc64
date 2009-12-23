/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: apimutexlock.c,v 1.4 2009/11/29 13:51:52 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/apimutex.h>

void _API_Mutex_Lock(
  API_Mutex_Control *the_mutex
)
{
  ISR_Level level;

  _ISR_Disable( level );

  _CORE_mutex_Seize(
    &the_mutex->Mutex,
    the_mutex->Object.id,
    true,
    0,
    level
  );
}
