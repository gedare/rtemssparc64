/*
 *  RTEMS Object Helper -- Get Least Valid API Number
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsobjectidapiminimum.c,v 1.1 2008/01/29 21:52:20 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/rtems/types.h>

#undef rtems_object_id_api_minimum
uint32_t rtems_object_id_api_minimum(void)
{
  return OBJECTS_INTERNAL_API;
}
