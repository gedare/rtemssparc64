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
 *  $Id: clock06.c,v 1.9 2009/05/10 20:15:54 joel Exp $
 */

#include <time.h>

#ifndef _POSIX_CPUTIME
#error "rtems is supposed to have clock_getenable_attr"
#endif
#ifndef _POSIX_THREAD_CPUTIME
#error "rtems is supposed to have clock_getenable_attr"
#endif

void test( void );

void test( void )
{
  clockid_t  clock_id = 0;
  int        attr;
  int        result;

  result = clock_getenable_attr( clock_id, &attr );
}
