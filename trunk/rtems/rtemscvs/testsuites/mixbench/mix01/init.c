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
 *  $Id: init.c,v 1.17 2009/10/02 11:50:39 joel Exp $
 */

#define CONFIGURE_INIT
#include "system.h"
#include "../../common/magic-instruction.h"


#include "crc.h"
#include <errno.h>
#include "../../common/allow.h"

#define PERMISSION_RECORD_SIZE sizeof(packed_permission_rec)
#define CONTMGR_PERM_STACKFIRST 1
#define CONTMGR_SINGLESTACKRANGE 1
#define CONTMGR_PERM_USEFETCH 1

//#define QUICK_RUN

int quicksort_main(int argc, char *argv[]);
int susan_main(int argc,char* argv[]);
int dijkstra_main(int argc,char* argv[]);

void b(int * buff)
{
	int i=0;
	unsigned long long * bb = (unsigned long long *) malloc(sizeof(unsigned long long));
	for(;i<10;i++)
		buff[i] = i;

	ALLOW(bb,sizeof(unsigned long long),3LL);
}


void a(int * buff)
{
	int i=0;
	for(;i<10;i++)
		buff[i] = i;
	b(buff);
}

int cccc(void)
{
	int i=0,s=0;
	int abc[10];
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

rtems_task CRCTaskFunction( rtems_task_argument argument)
{
  int times = 1;
//  printf( "\n\n*** crc benchmark ***\n" );
/*
  int i;
  unsigned long long int size = 0xAA;
  int sum = 0;
  char * buff = (char *) malloc(size);
  
   for(i=0;i<100;i++)
  	buff[i] = i; 
  for(i=0;i<100;i++)
  	 sum += buff[i];
  for(i=0;i<10;i++)
	  ALLOW(buff + i*(size+8),size,3LL); //perm uses 2 encoded bits
   a(buff);	  
   //rtems_task_suspend(RTEMS_SELF);
   //rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
   cccc();
  for(i=0;i<10;i++)
	  ALLOW(buff + i*(size+8),size,3LL); //perm uses 2 encoded bits
  free(buff);
*/
 #if !defined(QUICK_RUN)
  while(times){
	  char in_file[20] = "/image.img";

	  DWORD crc;
	  long charcnt;

	  crc32file(in_file,&crc, &charcnt);
	  printf("%08lX %7ld %s\n", crc, charcnt, in_file);

	  times--;
  }
#endif

  //printf( "\n\n*** end crc benchmark ***\n" );	
  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}


rtems_task DijkstraTaskFunction( rtems_task_argument argument)
{
	printf( "\n\n*** dijkstra benchmark ***\n" );

#if !defined(QUICK_RUN)

  char *argv[] = {"dijkstra","/input.dat"};

  dijkstra_main(2,argv);

  printf( "*** end of dijkstra benchmark ***\n" );
#endif

  
  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}


rtems_task QuickSortTaskFunction( rtems_task_argument argument)
{

  printf( "\n\n*** qsort_small benchmark ***\n" );

#if !defined(QUICK_RUN)

  char *argv[] = {"qsort_small","/input_small.dat"};

  quicksort_main(2,argv);
  

  printf( "*** end of qsort_small benchmark ***\n" );
#endif

  
  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}


rtems_task SusanCornersTaskFunction( rtems_task_argument argument)
{

  printf( "\n\n*** susan_corners benchmark ***\n" );

#if !defined(QUICK_RUN)

  char * argv[] = {"susan_corners","/input_small.pgm", "/susan_corners.pgm","-c"};

  susan_main(4,argv);

  printf( "*** end of susan_corners benchmark ***\n" );
#endif

  
  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}

rtems_task SusanEdgesTaskFunction( rtems_task_argument argument)
{

  printf( "\n\n*** susan_edges benchmark ***\n" );
#if !defined(QUICK_RUN)
  char * argv[] = {"susan_edges","/input_small.pgm", "/susan_edges.pgm","-e"};

  susan_main(4,argv);

  printf( "*** end of susan_edges benchmark ***\n" );
#endif

  
  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}

rtems_task TaskFunction(  rtems_task_argument argument)
{
  printf( "nothing %d !\n", (int) argument); 
  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_time_of_day time;
  rtems_status_code status;

  
  printf("Unpacking tar filesystem\nThis may take awhile...\n");
	if(Untar_FromMemory((void*)FileSystemImage, FileSystemImage_size) != 0) {
	  printf("Can't unpack tar filesystem\n");
	  exit(1);
	}

  puts( "\n\n*** TEST 1 ***" );

  build_time( &time, 12, 31, 1988, 9, 0, 0, 0 );
  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] = rtems_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] = rtems_build_name( 'T', 'A', '3', ' ' );
  //Task_name[ 4 ] = rtems_build_name( 'T', 'A', '4', ' ' );
  Task_name[ 4 ] = rtems_build_name( 'K', 'I', 'L', 'L' );
  

  status = rtems_task_create(
     Task_name[ 1 ],
     1,
     5*RTEMS_MINIMUM_STACK_SIZE ,
     //RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_NO_PREEMPT|RTEMS_NO_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
  directive_failed( status, "rtems_task_create of TA1" );

  /*
  status = rtems_task_create(
     Task_name[ 2 ],
     1,
     5*RTEMS_MINIMUM_STACK_SIZE ,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "rtems_task_create of TA2" );
*/
  
   status = rtems_task_create(
     Task_name[ 2 ],
     1,
     5*RTEMS_MINIMUM_STACK_SIZE ,
     //RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_NO_PREEMPT|RTEMS_NO_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "rtems_task_create of TA3" );

  status = rtems_task_create(
     Task_name[ 3 ],
     1,
     5*RTEMS_MINIMUM_STACK_SIZE ,
     //RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_NO_PREEMPT|RTEMS_NO_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 3 ]
  );
  directive_failed( status, "rtems_task_create of KILL" );
  
  status = rtems_task_create(
	 Task_name[ 4 ],
	 2,
	 5*RTEMS_MINIMUM_STACK_SIZE ,
	 //RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_NO_PREEMPT|RTEMS_NO_TIMESLICE,
	 RTEMS_DEFAULT_ATTRIBUTES,
	 &Task_id[ 4 ]
  );
  directive_failed( status, "rtems_task_create of KILL" );
  

  puts( "\n\n*** TEST START *** \n" );

  MAGIC_BREAKPOINT;

  status = rtems_task_start( Task_id[ 1 ], CRCTaskFunction, 1 );
  directive_failed( status, "rtems_task_start of TA1" );

  //status = rtems_task_start( Task_id[ 2 ], QuickSortTaskFunction, 2 );
  //status = rtems_task_start( Task_id[ 2 ], DijkstraTaskFunction, 2 );
  //directive_failed( status, "rtems_task_start of TA2" );

  status = rtems_task_start( Task_id[ 2 ], SusanEdgesTaskFunction, 2 );
  directive_failed( status, "rtems_task_start of TA2" );

  status = rtems_task_start( Task_id[ 3 ], SusanCornersTaskFunction, 3 );
  directive_failed( status, "rtems_task_start of TA3" );

  status = rtems_task_start( Task_id[ 4 ], KillTaskFunction, 4 );
  directive_failed( status, "rtems_task_start of KILL" );
  
  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );


}






