/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: system.h,v 1.5 2009/04/28 06:39:01 ccj Exp $
 */

#include <rtems.h>

/* configuration information */

#include <bsp.h> /* for device driver prototypes */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY TRUE

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_MAXIMUM_SEMAPHORES       5

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT_TASK_STACK_SIZE    (RTEMS_MINIMUM_STACK_SIZE * 2)
#define CONFIGURE_EXTRA_TASK_STACKS       RTEMS_MINIMUM_STACK_SIZE

#define CONFIGURE_USE_MINIIMFS_AS_BASE_FILESYSTEM

#include <rtems/confdefs.h>

/* end of include file */
