/*
 *  exit Shell Command Implmentation
 *
 *  Author: Fernando RUIZ CASAS
 *  Work: fernando.ruiz@ctv.es
 *  Home: correo@fernando-ruiz.com
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: main_exit.c,v 1.3 2007/12/17 22:39:28 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/shell.h>
#include "internal.h"

rtems_shell_alias_t rtems_shell_EXIT_Alias = {
  "logoff",                   /* command */
  "exit"                      /* alias */
};
