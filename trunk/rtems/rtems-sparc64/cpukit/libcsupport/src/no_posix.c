/*
 *  Marginal implementations of some POSIX API routines
 *  to be used when POSIX is disabled.
 *
 *    + kill
 *    + _kill_r
 *    + __kill
 *    + sleep
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: no_posix.c,v 1.8 2004/04/18 06:05:34 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>

#include <unistd.h>

/*
 *  These are directly supported (and completely correct) in the posix api.
 */

#if !defined(RTEMS_POSIX_API)
int kill( pid_t pid, int sig )
{
  return 0;
}

#if defined(RTEMS_NEWLIB)
#include <reent.h>

int _kill_r( struct _reent *ptr, pid_t pid, int sig )
{
  return 0;
}
#endif
#endif

int __kill( pid_t pid, int sig )
{
  return 0;
}


/*
 *  3.4.3 Delay Process Execution, P1003.1b-1993, p. 81
 *
 *  $Id: no_posix.c,v 1.8 2004/04/18 06:05:34 ralf Exp $
 */

#include <time.h>
#include <unistd.h>

#include <rtems.h>

#if !defined(RTEMS_POSIX_API)
unsigned int sleep(
  unsigned int seconds
)
{
  rtems_status_code status;
  rtems_interval    ticks_per_second;
  rtems_interval    ticks;

  status = rtems_clock_get(
  RTEMS_CLOCK_GET_TICKS_PER_SECOND,
  &ticks_per_second
  );

  ticks = seconds * ticks_per_second;

  status = rtems_task_wake_after( ticks );

  /*
   *  Returns the "unslept" amount of time.  In RTEMS signals are not
   *  interruptable, so tasks really sleep all of the requested time.
   */

  return 0;
}
#endif
