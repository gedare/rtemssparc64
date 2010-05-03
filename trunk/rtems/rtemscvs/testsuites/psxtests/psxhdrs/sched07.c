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
 *  $Id: sched07.c,v 1.11 2009/05/10 20:15:56 joel Exp $
 */

#include <sched.h>

#ifndef _POSIX_PRIORITY_SCHEDULING
#error "rtems is supposed to have sched_get_priority_min"
#endif

void test( void );

void test( void )
{
  int    policy;
  int    result;

  policy = SCHED_RR;
  policy = SCHED_FIFO;
  policy = SCHED_OTHER;
#ifdef _POSIX_SPORADIC_SERVER
  policy = SCHED_SPORADIC;
#endif

  result = sched_get_priority_min( policy );
}