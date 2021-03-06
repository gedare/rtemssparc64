/*
 *  Dual Port Memory Manager -- Instantiate Data
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: dpmemdata.c,v 1.1 2007/05/21 23:19:20 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

/* instantiate semaphore data */
#define RTEMS_DPMEM_EXTERN

#include <rtems/system.h>
#include <rtems/rtems/dpmem.h>
