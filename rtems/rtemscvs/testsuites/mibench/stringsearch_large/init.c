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

int stringsearch_main(int argc, char* argv[]);

rtems_task Init(
  rtems_task_argument ignored
)
{
  /* no file input for stringsearch */

  printf( "\n\n*** stringsearch benchmark ***\n" );

  char *argv[] = {"stringsearch_small"}; /* small */
  //char *argv[] = {"stringsearch_large"}; /* large */

  stringsearch_main(1,argv);
  
  printf( "*** end of stringsearch benchmark ***\n" );
  exit( 0 );
}

