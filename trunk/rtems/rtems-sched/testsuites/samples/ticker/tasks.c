/*  Test_task
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tasks.c,v 1.14 2009/08/12 20:50:38 joel Exp $
 */

#include "system.h"

rtems_task Test_task(
  rtems_task_argument unused
)
{
  rtems_id          tid;
  rtems_time_of_day time;
  uint32_t    task_index;
  rtems_status_code status;

  status = rtems_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  task_index = task_number( tid );
  for ( ; ; ) {
    status = rtems_clock_get_tod( &time );
    if ( time.second >= 35 ) {
      puts( "*** END OF CLOCK TICK TEST ***" );
      rtems_test_exit( 0 );
    }
    put_name( Task_name[ task_index ], FALSE );
    print_time( " - rtems_clock_get_tod - ", &time, "\n" );
    status = rtems_task_wake_after(
      task_index * 5 * rtems_clock_get_ticks_per_second() );
  }
}
