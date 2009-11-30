/*
 *  Rate Monotonic Manager -- Get Status
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ratemongetstatus.c,v 1.15 2009/10/30 17:54:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/rtems/ratemon.h>
#include <rtems/score/thread.h>

#if defined(RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS) || \
    defined(RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS)
  #include <rtems/score/timespec.h>
#endif

/*PAGE
 *
 *  rtems_rate_monotonic_get_status
 *
 *  This directive allows a thread to obtain status information on a
 *  period.
 *
 *  Input parameters:
 *    id     - rate monotonic id
 *    status - pointer to status control block
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 *
 */

rtems_status_code rtems_rate_monotonic_get_status(
  Objects_Id                           id,
  rtems_rate_monotonic_period_status  *status
)
{
  Objects_Locations              location;
  Rate_monotonic_Control        *the_period;
  #if defined(RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS) || \
      defined(RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS)
    Timestamp_Control            uptime;
    Timestamp_Control            temp;
  #endif

  if ( !status )
    return RTEMS_INVALID_ADDRESS;

  the_period = _Rate_monotonic_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      status->owner = the_period->owner->Object.id;
      status->state = the_period->state;

      if ( status->state == RATE_MONOTONIC_INACTIVE ) {
        #ifdef RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS
          _Timespec_Set_to_zero( &status->since_last_period );
        #else
          status->since_last_period = 0;
        #endif
        #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
          _Timespec_Set_to_zero( &status->executed_since_last_period );
        #else
          status->executed_since_last_period = 0;
        #endif
      } else {
        /*
         *  Both nanoseconds granularity options have to know the uptime.
         *  This lets them share one single invocation of _TOD_Get_uptime().
         */
        #if defined(RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS) || \
            defined(RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS)
          _TOD_Get_uptime( &uptime );
        #endif

        #ifdef RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS
          _Timestamp_Subtract( &the_period->time_at_period, &uptime, &temp );
          _Timestamp_To_timespec( &temp, &status->since_last_period );
        #else
          status->since_last_period =
            _Watchdog_Ticks_since_boot - the_period->time_at_period;
        #endif

        #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
          _Timestamp_Subtract(
            &_Thread_Time_of_last_context_switch,
            &uptime,
            &temp
          );
          _Timestamp_To_timespec( &temp, &status->executed_since_last_period );
        #else
          status->executed_since_last_period =
            the_period->owner->cpu_time_used -
            the_period->owner_executed_at_period;
        #endif
      }

      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:            /* should never return this */
#endif
    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}
