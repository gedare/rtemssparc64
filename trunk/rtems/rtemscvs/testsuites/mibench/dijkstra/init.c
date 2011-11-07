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
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"
#include <stdio.h>
#include "../../common/magic-instruction.h"

#include "dijkstra.h"



rtems_task Init(
  rtems_task_argument ignored
)
{


  char in_file[20] = "/input.dat";

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  //FILE * fp= fopen(in_file,"w");
  //int  n = fwrite(FileSystemImage + 0x400,1, FileSystemImage_size - 0x400, fp);
  //fclose(fp);

  //printf( "Wrote: intended=%d wrote=%d error=%d\n",FileSystemImage_size - 0x400, n,errno);

  char *argv[] = {"dijkstra",in_file};
  printf( "\n\n*** dijkstra benchmark ***\n" );

  

  MAGIC_BREAKPOINT;
  
  dijkstra_main(2,argv);

  MAGIC_BREAKPOINT;

  printf( "*** end of dijkstra benchmark ***\n" );
  exit( 0 );
}

