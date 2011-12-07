/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems.h>

#include <stdio.h>
#include <stdlib.h>

#include <generated.h>

/* functions */
rtems_task Init(
  rtems_task_argument argument
);

/* configuration information */
#include <bsp.h> /* for device driver prototypes */

/* drivers */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
//#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_BARRIERS 1

/* tasks */
#define CONFIGURE_MAXIMUM_TASKS             1+NUM_TASKS
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS NUM_TASKS*((NUM_FUNCTIONS+19)/20*RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_TICKS_PER_TIMESLICE       1
#define CONFIGURE_MICROSECONDS_PER_TICK   RTEMS_MILLISECONDS_TO_MICROSECONDS(5)

#define CONFIGURE_INIT_TASK_PRIORITY 250

#include <rtems/confdefs.h>
