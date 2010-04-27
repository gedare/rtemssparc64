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
 *  $Id: timer03.c,v 1.10 2009/05/10 20:15:57 joel Exp $
 */

#include <time.h>

#ifndef _POSIX_TIMERS
#error "rtems is supposed to have timer_settime"
#endif

void test( void );

void test( void )
{
  timer_t           timerid = 0;
  int               flags;
  struct itimerspec value;
  struct itimerspec ovalue;
  int               result;

  value.it_interval.tv_sec = 0;
  value.it_interval.tv_nsec = 0;
  value.it_value.tv_sec = 0;
  value.it_value.tv_nsec = 0;

  flags = TIMER_ABSTIME;
  flags = CLOCK_REALTIME;

  result = timer_settime( timerid, flags, &value, &ovalue );
}
