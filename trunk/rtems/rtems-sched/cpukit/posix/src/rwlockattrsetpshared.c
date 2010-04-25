/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rwlockattrsetpshared.c,v 1.2 2007/12/17 16:19:14 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

/*PAGE
 *
 *  RWLock Attributes Set Process Shared
 */

int pthread_rwlockattr_setpshared(
  pthread_rwlockattr_t *attr,
  int                    pshared
)
{
  if ( !attr )
    return EINVAL;

  if ( !attr->is_initialized )
    return EINVAL;

  switch ( pshared ) {
    case PTHREAD_PROCESS_SHARED:
    case PTHREAD_PROCESS_PRIVATE:
      attr->process_shared = pshared;
      return 0;

    default:
      return EINVAL;
  }
}
