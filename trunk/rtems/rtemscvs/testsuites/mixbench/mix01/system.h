/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *	
 */


#include <tmacros.h>

#include "FileSystemImage.h"
#include "FileSystemImage.h"
#include <rtems/untar.h>


/* functions */

rtems_task Init(
  rtems_task_argument argument
);

#if 1
void task_container_switch (  Thread_Control * a,  Thread_Control * b);
bool task_container_create (Thread_Control * executing,  Thread_Control * creating);

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

#else

void task_tlb_switch (  Thread_Control * a,  Thread_Control * b);
bool task_tlb_create (Thread_Control * executing,  Thread_Control * creating);

#define CONFIGURE_INITIAL_EXTENSIONS \
  { \
    task_tlb_create,   		 /* create  */ \
    NULL,                    /* start   */ \
    NULL,                    /* restart */ \
    NULL,                    /* delete  */ \
    task_tlb_switch,   		 /* switch  */ \
    NULL,                    /* begin   */ \
    NULL,                    /* exitted */ \
    NULL          			 /* fatal   */ \
  }

#endif


/* configuration information */
//#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY 1

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

/* filesystem */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 40
#define CONFIGURE_IMFS_MEMFILE_BYTES_PER_BLOCK 512

/* tasks */
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_TICKS_PER_TIMESLICE       1
#define CONFIGURE_MICROSECONDS_PER_TICK		RTEMS_MILLISECONDS_TO_MICROSECONDS(10)



#define CONFIGURE_INIT_TASK_STACK_SIZE 20*RTEMS_MINIMUM_STACK_SIZE

#define CONFIGURE_EXTRA_TASK_STACKS         (20 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_TASKS             7

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN rtems_id   Task_id[ 6 ];         /* array of task ids */
TEST_EXTERN rtems_name Task_name[ 6 ];       /* array of task names */

/* end of include file */


