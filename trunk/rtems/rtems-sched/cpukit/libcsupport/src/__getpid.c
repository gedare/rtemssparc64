/*
 *  Some C Libraries reference this routine since they think getpid is
 *  a real system call.
 *
 *  $Id: __getpid.c,v 1.3 2004/04/15 13:24:45 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

pid_t __getpid(void)
{
  return getpid();
}
