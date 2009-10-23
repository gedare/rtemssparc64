/*
 *  RTEMS Object Helper -- Get Greatest Valid Class for an API
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsobjectapimaximumclass.c,v 1.2 2009/07/07 23:33:17 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/rtems/types.h>

unsigned int rtems_object_api_maximum_class(
  uint32_t api
)
{
  return _Objects_API_maximum_class(api);
}
