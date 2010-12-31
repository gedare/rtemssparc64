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
 */

#define CONFIGURE_INIT
#include "system.h"
#include <stdio.h>

#include "params.h"

/* Instantiate globals */
uint32_t  tasks_completed;
uint32_t  deadlines_missed;
rtems_id  tasks_complete_sem;
rtems_id  final_barrier;

/* timing hack */
#include <bsp.h>
#include <boot/ofw.h>
#define CPU_FREQ (5000000)
static unsigned int get_Frequency(void)
{
	phandle root = ofw_find_device("/");
	unsigned int freq;
	if (ofw_get_property(root, "clock-frequency", &freq, sizeof(freq)) <= 0) {
		printk("Unable to determine frequency, default: 0x%x\n",CPU_FREQ);
		return CPU_FREQ;
	}
	return freq;
}
uint32_t  Instructions_per_us;


rtems_task Init(
  rtems_task_argument ignored
)
{
  int               err = 0;
  uint32_t          index;
  rtems_status_code status;

  /* initialize globals */
  tasks_completed = 0;
  deadlines_missed = 0;

  /* setup dijkstra problem space */
  err = dijkstra_setup();
  if ( err ) {
    printf("Unable to setup dijkstra problem space\n");
    while(1);
  }

  build_task_name();

  /* Create semaphores (barriers) */
  status = rtems_semaphore_create(
      rtems_build_name( 'S', 'E', 'M', '0' ),  /* name = SEM0 */
      0,                                       /* locked */
      RTEMS_BINARY_SEMAPHORE | RTEMS_FIFO,     /* mutex w/desired discipline */
      0,                                       /* IGNORED */
      &tasks_complete_sem
    );
  directive_failed( status, "rtems_semaphore_create" );

  status = rtems_semaphore_create(
      rtems_build_name( 'S', 'E', 'M', '1' ),  /* name = SEM1 */
      0,                                       /* locked */
      RTEMS_SIMPLE_BINARY_SEMAPHORE,           /* mutex w/desired discipline */
      0,                                       /* IGNORED */
      &final_barrier
    );
  directive_failed( status, "rtems_semaphore_create" );

  /* Create task(s) */
  for ( index = 1 ; index <= NUM_PERIODIC_TASKS ; index++ ) {
    status = rtems_task_create(
      Task_name[ index ],
      Priorities[ index ],
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Task_id[ index ]
    );
    directive_failed( status, "rtems_task_create loop" );
  }
  for ( index = NUM_PERIODIC_TASKS+1; index <= NUM_TASKS; index++ ) {
    status = rtems_task_create(
      Task_name[ index ],
      Priorities[ index ],
      RTEMS_MINIMUM_STACK_SIZE*3,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Task_id[ index ]
    );
    directive_failed( status, "rtems_task_create loop" );
  }

  /* Start periodic tasks */
  for ( index = 1 ; index <= NUM_PERIODIC_TASKS ; index++ ) {
    status = rtems_task_start( Task_id[ index ], Tasks_Periodic, index );
    directive_failed( status, "rtems_task_start loop" );
  }

  /* Start dijkstra (background) tasks */
  for ( index = NUM_PERIODIC_TASKS+1 ; index <= NUM_TASKS ; index++ ) {
    status = rtems_task_start( Task_id[ index ], Tasks_Dijkstra, index );
    directive_failed( status, "rtems_task_start loop" );
  }

  /* sleep while holding semaphore: is 10 ticks enough for 
   * created/started tasks to finish initializing? */
  rtems_task_wake_after( 10 );

  /* protected by semaphore */
  asm volatile("break_start_opal:");

  /* release all of the waiting tasks */
  status = rtems_semaphore_flush( tasks_complete_sem );
  directive_failed( status, "rtems_semaphore_flush" );

  status = rtems_semaphore_release( tasks_complete_sem );
  directive_failed( status, "rtems_semaphore_release" );
  
  /* Should block forever */
  status = rtems_semaphore_obtain( final_barrier, RTEMS_DEFAULT_OPTIONS, 0 );
  directive_failed( status, "rtems_semaphore_obtain" );

  puts("Init killing self\n");

  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}

