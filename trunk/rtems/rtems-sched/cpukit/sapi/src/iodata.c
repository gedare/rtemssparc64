/*
 *  RTEMS Task Manager -- Instantiate Data
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: iodata.c,v 1.1 2007/05/29 19:56:35 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

/* instantiate RTEMS IO manager data */
#define SAPI_IO_EXTERN

#include <rtems/system.h>
#include <rtems/io.h>
