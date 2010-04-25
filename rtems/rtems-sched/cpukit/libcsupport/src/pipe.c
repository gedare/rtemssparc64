/*
 *  pipe() - POSIX 1003.1b 6.1.1 Create an Inter-Process Channel
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pipe.c,v 1.5 2008/10/14 15:06:25 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <sys/types.h>

extern int pipe_create(int filsdes[2]);

int pipe(
  int filsdes[2]
)
{
  if (filsdes == NULL) {
    errno = EFAULT;
    return -1;
  }
  return pipe_create(filsdes);
}
