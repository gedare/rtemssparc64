/*  Init
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"

#include "cbench.h"

rtems_task Init(
  rtems_task_argument ignored
)
{

  /* initialize benchmark */
  cbench_initialize();

  /* start measurement */
  asm volatile("break_start_opal:");

  /* optional warmup phase */
  cbench_warmup();

  /* uncomment if magic is used to reset stats after warmup */
  //MAGIC(1);

  /* workload */
  cbench_work();

  /* stop measurement */
  MAGIC_BREAKPOINT;

  /* unreached when magic is enabled */
  exit( 0 );
}

