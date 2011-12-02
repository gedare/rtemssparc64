
#include "../shared/cbench.h"

#include "generated.h"

#define CBENCH_DEBUG

#if defined(CBENCH_DEBUG)
#include <stdio.h>
#define DPRINTF(...) \
      printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

#include <tmacros.h>
rtems_id Barrier;
static rtems_id Tasks[NUM_TASKS];
static int Priorities[NUM_TASKS] = { 1, 2 }; /* Must be <250 */

void cbench_task_entry( rtems_task_argument argument )
{
  rtems_status_code status;
  CBENCH_PROLOG;
  int *rv;

  DPRINTF("%d: Entering barrier\n", argument);
  status = rtems_barrier_wait( Barrier, RTEMS_NO_TIMEOUT );
  directive_failed(status, "rtems_barrier_wait");

  DPRINTF("%d: Calling cbench_func_001\n", argument);
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
    DPRINTF("Creating task\n");
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
    DPRINTF("Starting task\n");
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

  DPRINTF("Releasing tasks\n");
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

