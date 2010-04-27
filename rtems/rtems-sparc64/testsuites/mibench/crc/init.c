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
#include "crc.h"
#include <errno.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  

  char in_file[20] = "/image.img";

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  //FILE * fp= fopen(in_file,"w");
  //int  n = fwrite(FileSystemImage + 0x400,1, FileSystemImage_size - 0x400, fp);
  //fclose(fp);
  
  //printf( "Wrote: intended=%d wrote=%d error=%d\n",FileSystemImage_size - 0x400, n,errno);

  	
  printf( "\n\n*** crc benchmark ***\n" );

  DWORD crc;
  long charcnt;

  crc32file(in_file,&crc, &charcnt);
  printf("%08lX %7ld %s\n", crc, charcnt, in_file);

  printf( "*** end of crc benchmark ***\n" );
  exit( 0 );
}

