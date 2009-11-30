/*
 *  RTEMS Object Helper -- Get Greatest Valid API Number
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsobjectidapimaximum.c,v 1.2 2009/11/23 14:53:04 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/rtems/types.h>

#undef rtems_object_id_api_maximum
int rtems_object_id_api_maximum(void)
{
  return OBJECTS_ITRON_API;
}
