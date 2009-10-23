/*
 *  _calloc_r Implementation
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: _calloc_r.c,v 1.3 2009/09/30 06:15:39 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(RTEMS_NEWLIB) && !defined(HAVE__CALLOC_R)
#include <sys/reent.h>
#include <stdlib.h>

void *_calloc_r(
  struct _reent *ignored __attribute__((unused)),
  size_t         elements,
  size_t         size
)
{
  return calloc( elements, size );
}
#endif
