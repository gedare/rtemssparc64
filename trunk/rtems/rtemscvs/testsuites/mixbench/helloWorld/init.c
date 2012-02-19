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

#if 0
#include <libcpu/mmu_support.h>
#include <rtems/libmmu.h>
#define ALIGN_UP(s, a)  (((s) + ((a) - 1)) & ~((a) - 1))


rtems_task TaskFunction(
  rtems_task_argument argument
)

{
 
  int i;
  void * base = (void *)0x0;
  int bound = 0x2000;
  rtems_status_code status;
  rtems_memory_protection_entry *mp_entry;
  rtems_memory_protection_region_descriptor r = {
    .name = "1",
    .base = 0xBB00000000,
    .bounds = 0x2000
  };
  rtems_memory_protection_domain *protection_domain = NULL;

  puts("go\n");
  status = rtems_memory_protection_initialize();
  directive_failed( status, "rtems_memory_protection_initialize" );
  status = rtems_memory_protection_initialize_domain( protection_domain, 100 );
  directive_failed( status, "rtems_memory_protection_initialize_domain" );

  for(i=0;i<100;i++,r.base+= 0x2000 )
  {
	status = rtems_memory_protection_create_entry(
      protection_domain,
      &r,
      RTEMS_MEMORY_PROTECTION_READ_PERMISSION,
      &mp_entry
  	);
    directive_failed( status, "rtems_memory_protection_create_entry" );
  }
  
  rtems_memory_protection_install_domain(protection_domain);
  rtems_memory_protection_uninstall_domain(protection_domain);

  puts("end\n");
  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}
#endif

int Recurse(int x) __attribute__ ((optimize(0)));

int Recurse(int x) 
{
	int a=x;
	//printf("%s %llx %d \n", __PRETTY_FUNCTION__, &a, a);
	if(!x) return x;
	return Recurse(--x);
}

rtems_task TaskFunction(   rtems_task_argument argument ) __attribute__ ((optimize(0)));

rtems_task TaskFunction(
  rtems_task_argument argument
) 
{
	int a=0;
	int i=0;
	rtems_status_code status;
	
	a = Recurse(20);
	for (i=0; i< 20;i++){
		printf("%s %llx %d %d\n", __PRETTY_FUNCTION__, &a,a, i);
	}
	puts("end\n");	
	status = rtems_task_delete( RTEMS_SELF );
	directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}



rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_time_of_day time;
  rtems_status_code status;

  puts( "\n\n*** TEST 1 XXY***" );

  build_time( &time, 12, 31, 1988, 9, 0, 0, 0 );
  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '0', '1' );
  Task_name[ 2 ] = rtems_build_name( 'T', 'A', '0', '2' );
  Task_name[ 3 ] = rtems_build_name( 'T', 'A', '0', '3' );
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
     Task_name[ 2 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE ,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "rtems_task_create of TA2" );

  status = rtems_task_create(
     Task_name[ 3 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE ,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 3 ]
  );
  directive_failed( status, "rtems_task_create of TA3" );

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

/*
  status = rtems_task_start( Task_id[ 2 ], TaskFunction, 2 );
  directive_failed( status, "rtems_task_start of TA2" );

  status = rtems_task_start( Task_id[ 3 ], TaskFunction, 3 );
  directive_failed( status, "rtems_task_start of TA3" );

*/
  status = rtems_task_start( Task_id[ 4 ], KillTaskFunction, 3 );
  directive_failed( status, "rtems_task_start of TA4" );

  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );


}

