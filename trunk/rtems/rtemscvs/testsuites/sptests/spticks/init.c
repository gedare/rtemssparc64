/*
 *  Copyright (c) 2010 Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems.h>
#include <tmacros.h>
#include <rtems/rtems/clock.h>

#define TICKS_TO_SPIN (4)

rtems_task Init(
  rtems_task_argument argument
)
{
  uint32_t start, stop;

  struct timespec start_ts, stop_ts, diff_ts;
  
  puts( "\n\n*** TEST TICKS ***" );

  rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &start);
  printf("ticks:%d\n", start);

  rtems_clock_get_uptime(&start_ts);
  while(FOREVER){
    rtems_clock_get_uptime(&stop_ts);
    _Timespec_Subtract(&start_ts,&stop_ts,&diff_ts);
    if (_Timespec_To_ticks(&diff_ts) >= TICKS_TO_SPIN) break;
  }

  rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &stop);
  printf("ticks:%d\n", stop);

  if ((stop-start) < TICKS_TO_SPIN) {
    puts("INIT - FAILED TO SPIN FOR TICKS_TO_SPIN CLOCK TICKS");
    rtems_test_exit(0);
  }

  puts( "*** END OF TEST TICKS ***" );

  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MICROSECONDS_PER_TICK 10000

#define CONFIGURE_MAXIMUM_TASKS             1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE


#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
