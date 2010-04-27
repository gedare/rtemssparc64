/*
 *  Rate Monotonic Manager -- Instantiate Data
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ratemondata.c,v 1.1 2007/05/21 23:19:20 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

/* instantiate RTEMS period data */
#define RTEMS_RATEMON_EXTERN

#include <rtems/system.h>
#include <rtems/rtems/ratemon.h>
