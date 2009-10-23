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
 *  $Id: time13.c,v 1.9 2009/05/10 20:15:57 joel Exp $
 */

#include <time.h>

void test( void );

void test( void )
{
  time_t     time;
  struct tm *timestruct_pointer;
  struct tm  timestruct;

  timestruct_pointer = localtime_r( &time, &timestruct );
}
