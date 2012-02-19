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
#include <errno.h>

#include "../../common/magic-instruction.h"
#include "../../common/allow.h"
#include "../../common/debug.h"




#define PERMISSION_RECORD_SIZE sizeof(packed_permission_rec)
#define CONTMGR_PERM_STACKFIRST 1
#define CONTMGR_SINGLESTACKRANGE 1
#define CONTMGR_PERM_USEFETCH 1

//#define QUICK_RUN
#define MIBENCHNAME "transitive"
#define MIBENCHMAIN char *argv[] = {"transitive","/tc05.in"}; transitive_main(2,argv);
#define MIBENCHPROT int transitive_main(int argc, char *argv[]);


MIBENCHPROT
rtems_task TaskFunction( rtems_task_argument argument)
{
  DPUTS( "\n\n*** mibench " MIBENCHNAME " benchmark ***\n" );

  MIBENCHMAIN

  DPUTS( "*** end of mibench " MIBENCHNAME " benchmark ***\n" );

  rtems_status_code status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}



rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_time_of_day time;
  rtems_status_code status;


  puts("Unpacking tar filesystem\nThis may take awhile...\n");
	if(Untar_FromMemory((void*)FileSystemImage, FileSystemImage_size) != 0) {
	  puts("Can't unpack tar filesystem\n");
	  exit(1);
	}

  puts( "\n\n*** TEST 1 ***" );

  build_time( &time, 12, 31, 1988, 9, 0, 0, 0 );
  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '0', '0' );
  Task_name[ 2 ] = rtems_build_name( 'T', 'A', '0', '1' );
  Task_name[ 3 ] = rtems_build_name( 'T', 'A', '0', '2' );
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


  DPUTS( "\n\n*** TEST START *** \n" );

  MAGIC_BREAKPOINT;

  status = rtems_task_start( Task_id[ 1 ], TaskFunction, 1 );
  directive_failed( status, "rtems_task_start of TA1" );

  status = rtems_task_start( Task_id[ 4 ], KillTaskFunction, 4 );
  directive_failed( status, "rtems_task_start of KILL" );

  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );


}




