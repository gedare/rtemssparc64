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

/* sha-specific includes */
#include <string.h>
#include <time.h>
#include "sha.h"

rtems_task Init(
  rtems_task_argument ignored
)
{

  FILE *fin;
  SHA_INFO sha_info;
  
  char in_file[20] = "/input_large.asc";

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  fin = fopen(in_file,"r");
  if (!fin) {
    printf("error opening %s for reading\n", in_file);
    exit(1);
  }

  printf( "\n\n*** sha benchmark ***\n" );

  sha_stream(&sha_info, fin);
  sha_print(&sha_info);
  fclose(fin);

  printf( "*** end of sha benchmark ***\n" );
  exit( 0 );
}

