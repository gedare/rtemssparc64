/**
 * @file rtems/mkrootfs.h
 *
 * RTEMS Root FS creation support.
 */

/*
  Copyright Cybertec Pty Ltd, 2000
  All rights reserved Cybertec Pty Ltd, 2000

  COPYRIGHT (c) 1989-1998.
  On-Line Applications Research Corporation (OAR).

  The license and distribution terms for this file may be
  found in the file LICENSE in this distribution or at

  http://www.rtems.com/license/LICENSE.

  This software with is provided ``as is'' and with NO WARRANTY.

  $Id: mkrootfs.h,v 1.8 2008/08/01 05:07:41 ralf Exp $
*/

#ifndef _RTEMS_MKROOTFS_H
#define _RTEMS_MKROOTFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems.h>

/*
 *  Builds the complete path, like "mkdir -p".
 */

int
rtems_rootfs_mkdir (const char *path, mode_t omode);

/*
 *  Appends the lines to the a file. Create the file
 *  and builds the path if it does not exist.
 */

int
rtems_rootfs_file_append (const char *file,
                          mode_t     omode,
                          const int  line_cnt,
                          const char **lines);

/*
 *  Helper for bulding an /etc/hosts file.
 */

int
rtems_rootfs_append_host_rec (unsigned long cip,
                              const char    *cname,
                              const char    *dname);

/*
 * Create a few common directories, plus a :
 * /etc/passwd, /etc/group, /etc/host.conf, and
 * /etc/hosts file.
 */

int
rtems_create_root_fs ( void );

#ifdef __cplusplus
}
#endif

#endif
