/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is called from init_exec and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"

int main(int argc, char *argv[]);

rtems_task Init(
  rtems_task_argument ignored
)
{
  /* no file input for fft */

  printf( "\n\n*** fft inverse benchmark ***\n" );

  char *argv[] = {"fft_small","4","4096", "-i"}; /* small */
  //char *argv[] = {"fft_large","8","32768", "-i"}; /* large */

  main(4,argv);
  
  printf( "*** end of fft inverse benchmark ***\n" );
  exit( 0 );
}

