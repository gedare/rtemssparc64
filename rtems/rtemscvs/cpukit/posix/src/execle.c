/*
 *  execle() - POSIX 1003.1b 3.1.2
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: execle.c,v 1.5 2008/12/05 06:44:40 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef HAVE_EXECLE

#include <errno.h>
#include <rtems/seterr.h>

int execle(
  const char *path,
  char const *arg,
  ...
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}

#endif
