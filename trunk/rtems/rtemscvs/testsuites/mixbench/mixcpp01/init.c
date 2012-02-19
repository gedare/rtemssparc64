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
#include "../../common/allow.h"

rtems_task TaskFunction(   rtems_task_argument argument ) __attribute__ ((optimize(0)));
int testAnotherClass(int x);
int testSimpleClass();


rtems_task TaskFunction(
  rtems_task_argument argument
) 
{
	rtems_status_code status;

	int i=0;
	int a = testSimpleClass();
	int b = testAnotherClass(5);
	for(i=0;i<200;i++)
		printf("%d %d\n",a, b);
	
	//puts("end\n");	
	status = rtems_task_delete( RTEMS_SELF );
	directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}


rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_time_of_day time;
  rtems_status_code status;

  //puts( "\n\n*** TEST 1 XXY***" );

  build_time( &time, 12, 31, 1988, 9, 0, 0, 0 );
  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '0', '1' );
  Task_name[ 4 ] = rtems_build_name( 'K', 'I', 'L', 'L' );

  status = rtems_task_create(
     Task_name[ 1 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE ,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
  directive_failed( status, "rtems_task_create of TA1" );

  status = rtems_task_create(
     Task_name[ 4 ],
     2,
     RTEMS_MINIMUM_STACK_SIZE ,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 4 ]
  );
  directive_failed( status, "rtems_task_create of KILL" );

  puts( "\n\n*** TEST START *** \n" );

  MAGIC_BREAKPOINT;

  status = rtems_task_start( Task_id[ 1 ], TaskFunction, 1 );
  directive_failed( status, "rtems_task_start of TA1" );

  status = rtems_task_start( Task_id[ 4 ], KillTaskFunction, 3 );
  directive_failed( status, "rtems_task_start of TA4" );

  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );


}


