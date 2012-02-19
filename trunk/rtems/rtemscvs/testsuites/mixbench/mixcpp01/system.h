/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *	
 */


#include <tmacros.h>

/* functions */

rtems_task Init(
  rtems_task_argument argument
);

rtems_task Task_1_through_3(
  rtems_task_argument argument
);

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_TICKS_PER_TIMESLICE       1


#define CONFIGURE_EXTRA_TASK_STACKS         (5 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_TASKS             5

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN rtems_id   Task_id[ 5 ];         /* array of task ids */
TEST_EXTERN rtems_name Task_name[ 5 ];       /* array of task names */

/* end of include file */


