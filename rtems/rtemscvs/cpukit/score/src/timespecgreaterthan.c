/**
 *  @file  score/src/timespecgreaterthan.c
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timespecgreaterthan.c,v 1.4 2009/11/29 13:51:52 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>

#include <rtems/system.h>
#include <rtems/score/timespec.h>
#include <rtems/score/tod.h>

bool _Timespec_Greater_than(
  const struct timespec *lhs,
  const struct timespec *rhs
)
{
  if ( lhs->tv_sec > rhs->tv_sec )
    return true;

  if ( lhs->tv_sec < rhs->tv_sec )
    return false;

  /* ASSERT: lhs->tv_sec == rhs->tv_sec */
  if ( lhs->tv_nsec > rhs->tv_nsec )
    return true;

  return false;
}
