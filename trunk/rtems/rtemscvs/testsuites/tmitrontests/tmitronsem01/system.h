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
 *  $Id: system.h,v 1.6 2004/04/01 15:16:32 ralf Exp $
 */

#include <tmacros.h>
#include <rtems/itron.h>
#include <assert.h>

/* functions */

void ITRON_Init( void );

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_ITRON_TASKS         1
#define CONFIGURE_MAXIMUM_ITRON_SEMAPHORES  100
#define CONFIGURE_ITRON_INIT_TASK_TABLE

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN rtems_id Global_variable;   /* example global variable     */

/* end of include file */
