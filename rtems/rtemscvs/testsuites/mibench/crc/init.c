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
#include "../../common/allow.h"
#include "../../common/magic-instruction.h"


void b(char * buff)
{
	int i=0;
	unsigned long long * bb = (unsigned long long *) malloc(sizeof(unsigned long long));
	for(;i<10;i++)
		buff[i] = i;

	ALLOW(0xffff0,sizeof(unsigned long long),3LL);
}


void a(char * buff)
{
	int i=0;
	for(;i<10;i++)
		buff[i] = i;
	b(buff);
}

int cccc(void)
{
	int i=0,s=0;
	int* abc[10];
	for(;i<10;i++){
		abc[i] = i;
	}
	s=0;
	for(i=0;i<10;i++){
		s+=abc[i];
	}
	a(abc);
	
	return s;
}

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

 /* 
 	__asm__ ( "testpcd:"
  			"nop\n\t"
  			"nop\n\t"
  			"stx  %g1, [ %fp + 0x7ef ]\n\t"
  			"nop\n\t"
  			"impdep2  0, %g0, %g0, %g0\n\t"
			"nop\n\t"
			"nop\n\t"
	 );
*/

/*


  int i=0;
  unsigned long long int size = 0xAA;
  int sum = 0;

  
  char * buff = (char *) malloc(size);

  MAGIC_BREAKPOINT;
  for(i=0;i<100;i++)
  	buff[i] = i; 
  for(i=0;i<100;i++)
  	 sum += buff[i];
//  printf("buff=0x%x sum=%d\n", (void *)buff, sum);
  for(i=0;i<10;i++)
	  ALLOW(buff + i*(size+8),size,3LL); //perm uses 2 encoded bits
   a(buff);	  
   cccc();
  //crc32file(in_file,&crc, &charcnt);
  for(i=0;i<10;i++)
	  ALLOW(buff + i*(size+8),size,3LL); //perm uses 2 encoded bits
  free(buff);
  MAGIC_BREAKPOINT;  

  
	 __asm__ ( "testpcd:"
  			"nop\n\t"
  			"nop\n\t"
  			"nop\n\t"
  			"impdep2  0, %g0, %g0, %g0\n\t"
			"nop\n\t"
			"nop\n\t"
	 );
*/
	
  MAGIC_BREAKPOINT;
  crc32file(in_file,&crc, &charcnt);
  printf("%08lX %7ld %s\n", crc, charcnt, in_file);
  MAGIC_BREAKPOINT;

  printf( "*** end of crc benchmark ***\n" );
  exit( 0 );
}

