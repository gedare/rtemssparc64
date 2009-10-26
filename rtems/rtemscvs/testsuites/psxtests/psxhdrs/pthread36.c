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
 *  $Id: pthread36.c,v 1.9 2009/05/10 20:15:56 joel Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREAD_CPUTIME
#error "rtems is supposed to have pthread_attr_getcputime"
#endif

void test( void );

void test( void )
{
  pthread_attr_t  attr;
  int             clock_allowed;
  int             result;

  result = pthread_attr_getcputime( &attr, &clock_allowed );
}