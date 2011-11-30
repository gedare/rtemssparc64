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

void task_container_switch(  Thread_Control * a,  Thread_Control * b);
bool task_container_create(Thread_Control * executing,  Thread_Control * creating);


#define CONFIGURE_INITIAL_EXTENSIONS \
  { \
    task_container_create,   /* create  */ \
    NULL,                    /* start   */ \
    NULL,                    /* restart */ \
    NULL,                    /* delete  */ \
    task_container_switch,   /* switch  */ \
    NULL,                    /* begin   */ \
    NULL,                    /* exitted */ \
    NULL          			 /* fatal   */ \
  }


/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_TICKS_PER_TIMESLICE       1


#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_TASKS             3

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN rtems_id   Task_id[ 3 ];         /* array of task ids */
TEST_EXTERN rtems_name Task_name[ 3 ];       /* array of task names */

/* end of include file */


