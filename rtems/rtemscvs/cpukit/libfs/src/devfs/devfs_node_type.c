/*
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: devfs_node_type.c,v 1.2 2009/11/29 13:18:56 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "devfs.h"

int devFS_node_type(
  rtems_filesystem_location_info_t  *pathloc
)
{
  /*
   * There is only one type of node: device
   */

  return RTEMS_FILESYSTEM_DEVICE;
}


