/*
 *  16.1.6 Get Calling Thread's ID, p1003.1c/Draft 10, p. 152
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pthreadself.c,v 1.4 2003/09/04 18:54:25 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/thread.h>

pthread_t pthread_self( void )
{
  return _Thread_Executing->Object.id;
}
