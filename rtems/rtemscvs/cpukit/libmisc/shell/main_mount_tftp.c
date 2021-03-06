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
 *  $Id: main_mount_tftp.c,v 1.2 2007/12/17 22:39:29 joel Exp $
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
#include <rtems/fsmount.h>
#include "internal.h"

#include <rtems/ftpfs.h>
#include <rtems/tftp.h>

rtems_shell_filesystems_t rtems_shell_Mount_TFTP = {
  name:          "tftp",
  driver_needed: 0,
  fs_ops:        &rtems_tftp_ops,
  mounter:       rtems_shell_libc_mounter
};
