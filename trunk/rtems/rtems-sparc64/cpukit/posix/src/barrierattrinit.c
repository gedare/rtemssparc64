/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: barrierattrinit.c,v 1.3 2009/02/03 10:10:42 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>

/*PAGE
 *
 *  Barrier Attributes Initialization
 */

int pthread_barrierattr_init(
  pthread_barrierattr_t *attr
)
{
  if ( !attr )
    return EINVAL;

  attr->is_initialized = true;
  attr->process_shared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}
