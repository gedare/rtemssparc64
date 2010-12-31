/*  Tasks_Periodic
 *
 *  This routine serves as the body of a periodic task 
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

rtems_task Tasks_Periodic(
  rtems_task_argument argument
)
{
  rtems_id          rmid;
  rtems_id          test_rmid;
  rtems_status_code status;
  uint32_t          periods_executed = 0;
  uint32_t          local_deadlines_missed = 0;

  status = rtems_rate_monotonic_create( argument, &rmid );
  directive_failed( status, "rtems_rate_monotonic_create" );

  status = rtems_rate_monotonic_ident( argument, &test_rmid );
  directive_failed( status, "rtems_rate_monotonic_ident" );

  if ( rmid != test_rmid ) {
     printf( "RMID's DO NOT MATCH (0x%" PRIxrtems_id " and 0x%" 
         PRIxrtems_id ")\n", rmid, test_rmid );
     rtems_test_exit( 0 );
  }
  
  /* release all periodic tasks at the same time */
  /* Barrier: tasks will be released by the init function */
  status = rtems_semaphore_obtain( tasks_complete_sem, 
      RTEMS_DEFAULT_OPTIONS, 0 );

  rtems_task_wake_after( Phases[ argument ] ); /* typically 0 */

  /* Periodic Loop */
  while (FOREVER) {
    if (rtems_rate_monotonic_period(rmid, Periods[argument])==RTEMS_TIMEOUT) {
      printf("Deadline missed\n");
      local_deadlines_missed++;
    }
    
    /* active computing */
    busy_wait( Execution_us[argument], Instructions_per_us );
    periods_executed++;

    /* termination condition: based on amount of 'work' completed. */
    /* No termination:
    if ( periods_executed*Periods[argument] >= 2*MAX_PERIOD ) break; 
    */
  }

  printf("%d: Shouldn't get here\n", argument);

  status = rtems_semaphore_obtain( tasks_complete_sem, 
      RTEMS_DEFAULT_OPTIONS, 0 );
  directive_failed( status, "rtems_semaphore_obtain" );
    tasks_completed++;
    deadlines_missed += local_deadlines_missed;

    if ( NUM_TASKS == tasks_completed ) {
      if (deadlines_missed) {
        MAGIC(1);
      }
      MAGIC_BREAKPOINT;
      rtems_test_exit( 0 );
    }
  status = rtems_semaphore_release( tasks_complete_sem );
  directive_failed( status, "rtems_semaphore_release" );

  /* Should block forever */
  status = rtems_semaphore_obtain( final_barrier, RTEMS_DEFAULT_OPTIONS, 0 );
  directive_failed( status, "rtems_semaphore_obtain" );

  /* Shouldn't reach this */
  printf( "Killing task %d\n", argument);
  status = rtems_task_delete(RTEMS_SELF);
  directive_failed(status, "rtems_task_delete of RTEMS_SELF");
}
