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
   
  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  printf( "\n\n*** matrix benchmark ***\n" );

  /* NOTE: the original input sizes malloc a lot of memory */
#define FILE1 "/m01.in"
#define FILE2 "/m02.in"
#define FILE3 "/m03.in"

  char *argv[] = {"matrix",FILE3};

  main(2,argv);
  
  printf( "*** end of matrix benchmark ***\n" );
  exit( 0 );
}

