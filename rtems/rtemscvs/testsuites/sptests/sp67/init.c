/*
 *  Based upon test code posted on the RTEMS User's Mailing List
 *  by Sergio Faustino <sergio.faustino@edisoft.pt>:
 *
 *    http://www.rtems.org/pipermail/rtems-users/2009-June/005540.html
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.1 2009/10/10 15:03:30 joel Exp $
 */


#include <tmacros.h>

/*
 *  We have to extern this rather than use __RTEMS_VIOLATE_KERNEL_VISIBILITY__
 *  because this variable isn't actually in any .h.
 */
extern Watchdog_Interval _Timer_Server_ticks_last_time;

volatile bool _timer_passage_1 = FALSE;
volatile bool _timer_passage_2 = FALSE;

/*timer Routine*/
rtems_timer_service_routine TIMER_service_routine(
  rtems_id  ignored_id,
  void     *user_data
)
{
  bool *passed = (bool *)user_data;
  *passed = TRUE;
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_id          timer1;
  rtems_id          timer2;
  rtems_name        timer_name;

  puts( "\n\n*** TEST 67 ***" );

  /* build timer name*/

  /* create Timer */
  puts( "Init - create timer 1" );
  status = rtems_timer_create( rtems_build_name('T', 'M', '1', ' '), &timer1 );
  directive_failed( status, "rtems_timer_create #1" );

  puts( "Init - create timer 2" );
  status = rtems_timer_create( rtems_build_name('T', 'M', '2', ' '), &timer2 );
  directive_failed( status, "rtems_timer_create #1" );

  /* initiate timer server */
  puts( "Init - Initiate the timer server" );
  status = rtems_timer_initiate_server(
    RTEMS_MINIMUM_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES
  );
  directive_failed( status, "rtems_timer_initiate_server" );

  status = rtems_task_wake_after( 10 );
  directive_failed( status, "task wake_after" );

  status = rtems_timer_server_fire_after(
    timer1,
    0xffff,
    TIMER_service_routine,
    (void*) &_timer_passage_1
  );
  directive_failed( status, "rtems_timer_server_fire_after" );

  /* Make the timer server think that the ticks has wrapped */
  _Timer_Server_ticks_last_time = 100;

  status = rtems_task_wake_after( 10 );
  directive_failed( status, "task wake_after" );

  /* Make the timer server think that the ticks has wrapped */
  _Timer_Server_ticks_last_time = 200;

  status = rtems_timer_server_fire_after(
    timer2,
    0xffff,
    TIMER_service_routine,
    (void*) &_timer_passage_2
  );
  directive_failed( status, "rtems_timer_server_fire_after" );

  status = rtems_task_wake_after( 10 );
  directive_failed( status, "task wake_after" );

  if (!_timer_passage_1) {
    puts( "Timer 1 FAILED to fire after wrapping time");
    rtems_test_exit(0);
  }
  puts( "Server Timer 1 fired after wrapping ticks since boot-- OK");

  if (_timer_passage_2) {
    puts( "Timer 2 fired and should not have after wrapping time");
    rtems_test_exit(0);
  }

  puts( "*** END OF TEST 67 ***" );
  rtems_test_exit(0);
}

/* configuration stuff */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS              2
#define CONFIGURE_MAXIMUM_TIMERS             2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>