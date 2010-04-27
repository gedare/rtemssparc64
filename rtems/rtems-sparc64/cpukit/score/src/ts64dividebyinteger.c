/**
 * @file score/src/ts64dividebyinteger.c
*/

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ts64dividebyinteger.c,v 1.1 2008/12/09 21:27:35 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>

#include <rtems/system.h>
#include <rtems/score/timestamp.h>

#if defined(CPU_RTEMS_SCORE_TIMESTAMP_IS_INT64) && \
    !defined(CPU_RTEMS_SCORE_TIMESTAMP_INT64_INLINE)
void _Timestamp64_Divide_by_integer(
  Timestamp64_Control *_time,
  uint32_t             _iterations,
  Timestamp64_Control *_result
)
{
  *_result = *_time / _iterations;
}
#endif
