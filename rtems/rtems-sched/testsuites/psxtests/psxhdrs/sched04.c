/*
 *  This test file is used to verify that the header files associated with
 *  invoking this function are correct.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sched04.c,v 1.10 2009/05/10 20:15:56 joel Exp $
 */

#include <sched.h>

#ifndef _POSIX_PRIORITY_SCHEDULING
#error "rtems is supposed to have sched_getscheduler"
#endif

void test( void );

void test( void )
{
  pid_t  pid;
  int    result;

  pid = 0;

  result = sched_getscheduler( pid );
}
