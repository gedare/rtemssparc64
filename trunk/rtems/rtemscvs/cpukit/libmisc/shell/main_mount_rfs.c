/*
 *   Shell Command Implmentation
 *
 *  Author: Fernando RUIZ CASAS
 *  Work: fernando.ruiz@ctv.es
 *  Home: correo@fernando-ruiz.com
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: main_mount_rfs.c,v 1.1 2010/02/19 03:23:11 ccj Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <rtems.h>
#include <rtems/shell.h>
#include <rtems/rtems-rfs.h>
#include <rtems/fsmount.h>
#include "internal.h"

rtems_shell_filesystems_t rtems_shell_Mount_RFS = {
  name:          "rfs",
  driver_needed: 1,
  fs_ops:        &rtems_rfs_ops,
  mounter:       rtems_shell_libc_mounter
};

