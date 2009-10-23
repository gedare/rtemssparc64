/*
 *  Halt Command Implementation
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: main_halt.c,v 1.2 2009/04/14 13:41:33 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include <rtems.h>
#include <rtems/shell.h>
#include "internal.h"

int rtems_shell_main_halt(
  int   argc __attribute__((unused)),
  char *argv[] __attribute__((unused))
)
{
  exit(0);
  return 0;
}

rtems_shell_cmd_t rtems_shell_HALT_Command = {
  "halt",                                    /* name */
  "halt",                                    /* usage */
  "rtems",                                   /* topic */
  rtems_shell_main_halt,                     /* command */
  NULL,                                      /* alias */
  NULL                                       /* next */
};
