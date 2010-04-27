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
 *  $Id: proc03.c,v 1.10 2009/05/10 20:15:55 joel Exp $
 */

#include <unistd.h>
#include <sys/types.h>

void test( void );

void test( void )
{
  uid_t uid;

  uid = getuid();
}
