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
 *  $Id: signal10.c,v 1.9 2009/05/10 20:15:56 joel Exp $
 */

#include <signal.h>

void test( void );

void test( void )
{
  sigset_t  set;
  int       sig;
  int       result;

  result = sigwait( &set, &sig );
}
