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
#include "../../common/magic-instruction.h"

int transitive_main(int argc, char *argv[]);

rtems_task Init(
  rtems_task_argument ignored
)
{
   
  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  printf( "\n\n*** transitive benchmark ***\n" );

#define FILE1 "/tc01.in"
#define FILE2 "/tc02.in"
#define FILE3 "/tc03.in"
#define FILE4 "/tc04.in"
#define FILE5 "/tc05.in"

  char *argv[] = {"transitive",FILE5};
  MAGIC_BREAKPOINT;
  transitive_main(2,argv);
  MAGIC_BREAKPOINT;  
  printf( "*** end of transitive benchmark ***\n" );
  exit( 0 );
}

