
#include "../shared/cbench.h"

#include "generated.h"

#define CBENCH_DEBUG

#include <tmacros.h>
static rtems_id Barrier;
static rtems_id Tasks[NUM_TASKS];
static int Priorities[NUM_TASKS] = { 1, 2 }; /* Must be <250 */

void cbench_task_entry( rtems_task_argument argument )
{
  rtems_status_code status;
  CBENCH_PROLOG;
  int *rv;

  status = rtems_barrier_wait( Barrier, RTEMS_NO_TIMEOUT );
  directive_failed(status, "rtems_barrier_wait");

  FCALL( rv, cbench_func_001 );

  if ( rv )
    free( rv );

  CBENCH_EPILOG_VOID;
}

void cbench_initialize( ) {
  rtems_name        name = rtems_build_name('B','A','R','1');
  rtems_status_code status;
  int               i;

  /* create the barrier to control tasks execution */
  status = rtems_barrier_create(
    name,
    RTEMS_BARRIER_MANUAL_RELEASE,
    0,
    &Barrier
  );
  directive_failed(status, "rtems_barrier_create");

  /* create tasks */
  for ( i = 0; i < NUM_TASKS; i++ ) {
    status = rtems_task_create(
      rtems_build_name('T','A','0'+i/10,'0'+i%10),
      Priorities[i],
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Tasks[ i ]
    );
    directive_failed(status, "rtems_task_create" );
  }

  /* start tasks, should preempt this task */
  for ( i = 0; i < NUM_TASKS; i++ ) {
    status = rtems_task_start( Tasks[i], cbench_task_entry, i );
    directive_failed(status, "rtems_task_start" );
  }
}

void cbench_warmup( ) {
  // do nothing
}

void cbench_work( ) {
  rtems_status_code status;
  uint32_t          released;

  /* release the tasks. preempts this task. */
  status = rtems_barrier_release( Barrier, &released );
  directive_failed(status, "rtems_barrier_release");
  if ( released != NUM_TASKS ) {
    printf("ERROR: barrier released %" PRIu32 " < %d\n", released, NUM_TASKS);
  }

#ifdef CBENCH_DEBUG
  /* sleep for a bit to allow printfs to flush to console */
  rtems_task_wake_after(2);
#endif
}

